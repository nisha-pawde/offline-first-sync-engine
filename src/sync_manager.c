 #include <stdio.h>
 #include <unistd.h>
 #include <sys/shm.h>
 #include <sys/msg.h>
 #include <stdlib.h>
 #include "config.h"
 #include <fcntl.h>
 #include <string.h>
 #include "ipc.h"
 #include <pthread.h>
 #include <time.h>

 struct msg{
      long type;
      char text[100];
    };

 int msgid;
 char *data;
 int fd[2];
 int network_status = 0;
 pthread_mutex_t lock;

 void sync_pending_data(int pipe_fd){

      int file = open("data/local_data.txt", O_RDONLY);
      if(file < 0) return;

      char buffer[1024];
      int n = read(file, buffer, sizeof(buffer)-1);
      close(file);

      if(n <= 0) return;

      buffer[n] = '\0';

      char new_data[2048] = "";

      char *line = strtok(buffer, "\n");

      while(line != NULL){
            if(strstr(line, "PENDING")){
                     char data_only[100];
                     sscanf(line, "%s", data_only);

                     printf("SYNCING PENDING: %s\n", data_only);

                     write(pipe_fd, data_only, strlen(data_only));
                     write(pipe_fd, "\n", 1);

                     struct msg m;
                     m.type = MSG_DATA;
                     sprintf(m.text, "%s", data_only);
                     msgsnd(msgid, &m, sizeof(m.text), 0);

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

      int file_write = open("data/local_data.txt", O_WRONLY | O_TRUNC);
      if(file_write >= 0){
            write(file_write, new_data, strlen(new_data));
            close(file_write);
         }
   }

 void* network_thread(void *arg){

      while(1){
            sleep(3);

            pthread_mutex_lock(&lock);

            network_status = rand() % 2;

            if(network_status){
                  printf("NETWORK UP\n");
               }
            else{
                  printf("NETWORK DOWN\n");
               }
            pthread_mutex_unlock(&lock);
         }

      return NULL;
   }

 void* sync_thread(void *arg){

      while(1){

            pthread_mutex_lock(&lock);

            if(network_status){
                  struct msg m;
                  m.type = MSG_DATA;
                  
                  if(data != NULL && strlen(data) > 0){
                        sprintf(m.text, "%s", data);
                    }
                   else{
                         sprintf(m.text, "TEMP_DATA");
                     }

                  msgsnd(msgid, &m, sizeof(m.text), 0);

                  printf("Sent to MQ: %s\n", m.text);

                  struct msg ack; 

                  if(msgrcv(msgid, &ack, sizeof(ack.text), MSG_ACK, IPC_NOWAIT) > 0){
                         printf("ACK received from server\n");
                         sync_pending_data(fd[1]);
                     }
                  else{
                         printf("No ACK received\n");
                     }

               }
            else{
                  printf("Network DOWN -> Data pending\n");
               }

            pthread_mutex_unlock(&lock);
            sleep(2);
         }

      return NULL;
   }

int main(){

      srand(time(NULL));

      int shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
      if(shmid < 0){
           printf("Shared memory create failed\n");
           return 1;
        }

      data = (char*) shmat(shmid, NULL, 0);
      if(data == (char*) -1){
           printf("Shared memory attach failed\n");
           return 1;
         }

      if(strlen(data) == 0){
           strcpy(data, "TEMP_DATA");
         }

      msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);

      pipe(fd);

      if(fork() == 0){
            while(1){
                  char buf[100] = {0};
                  int n = read(fd[0], buf, sizeof(buf)-1);

                  if(n > 0){
                     buf[n] = '\0';
                     printf("PIPE READ: %s\n", buf);
                    }

                  sleep(1);
               }

            exit(0);
         }

      pthread_t t1, t2;
      pthread_mutex_init(&lock, NULL);

      pthread_create(&t1, NULL, network_thread, NULL);
      pthread_create(&t2, NULL, sync_thread, NULL);

      pthread_join(t1, NULL);
      pthread_join(t2, NULL);

      pthread_mutex_destroy(&lock);

      return 0;
   }

