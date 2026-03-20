
 #include <stdio.h>
 #include <unistd.h>
 #include <sys/shm.h>
 #include <sys/msg.h>
 #include <stdlib.h>
 #include "config.h"
 #include <fcntl.h>
 #include <string.h>
 #include "ipc.h"

 struct msg{
       long type;
       char text[100];
    };

 void sync_pending_data(int pipe_fd){

           int fd = open("data/local_data.txt", O_RDONLY);
           if(fd < 0) return;

           char buffer[1024];
           int n = read(fd, buffer, sizeof(buffer)-1);
           close(fd);

           if(n <= 0) return;

           buffer[n] = '\0';

           char new_data[2048] = "";

           char *line = strtok(buffer, "\n");

           while(line != NULL){
                 if(strstr(line, "PENDING")){
                          char data_only[100];
                          sscanf(line, "%s", data_only);
                          printf("SYNCING: %s\n", data_only);

                          write(pipe_fd, data_only, strlen(data_only));
                          write(pipe_fd, "\n", 1);
            
                          char synced[120];
                          sprintf(synced, "%s SYNC\n", data_only);

                          strcat(new_data, synced);

                     } 
                  else{
                      strcat(new_data, line);
                      strcat(new_data, "\n");
                    }

                  line = strtok(NULL, "\n");
             }

           int fd_write = open("data/local_data.txt", O_WRONLY | O_TRUNC);
           write(fd_write, new_data, strlen(new_data));
           close(fd_write);
      }




 int main(){

       int shmid = shmget(SHM_KEY, 1024, 0666);
       char *data = (char*) shmat(shmid, NULL, 0);

       int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
       struct msg m;
       

       while(1){
             int fd[2];
             pipe(fd);

             if(fork() == 0){
                   char buf[100];
                   read(fd[0], buf, sizeof(buf));
                   printf("PIPE READ: %s\n", buf);
                   exit(0);
                } else{
                     write(fd[1], data, 100);
                     printf("PIPE WRITE: %s\n", data);
                 }

             int network = rand() % 2;

             if(network){
                    m.type =  MSG_DATA;
                    sprintf(m.text, "%s", data);
                    msgsnd(msgid, &m, sizeof(m), 0);
                    printf("Sent to MQ: %s\n", m.text);
                } else{
                      printf("Network DOWN → Data pending\n");
                  }

             struct msg ack;
             if(msgrcv(msgid, &ack, sizeof(ack), MSG_ACK, IPC_NOWAIT) > 0){
                    printf("ACK received from server\n");
                    sync_pending_data(fd[1]);
                  }

              sleep(2);
           }

         return 0;
     }
