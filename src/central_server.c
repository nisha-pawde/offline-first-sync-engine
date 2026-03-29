
 #include <stdio.h>
 #include <sys/msg.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include "config.h"
 #include "ipc.h"
 #include <string.h>
 #include <stdlib.h>

 struct msg{
        long type;
        char text[100];
     };

 int main(){

        int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
        struct msg m;

        create_fifo();

        while(1){ 

               if(msgrcv(msgid, &m, sizeof(m.text), MSG_DATA, 0) > 0){
                     printf("Server received: %s\n", m.text);
  
                     int fd = open(CENTRAL_FILE, O_CREAT | O_WRONLY | O_APPEND, 0666);
                     if(fd >= 0){                   
                          write(fd, m.text, strlen(m.text));
                          write(fd, "\n", 1);
                          close(fd);
                         }
                     else{
                           printf("Central file open failed\n");
                        }

                     struct msg ack;
                     ack.type = MSG_ACK;
                     sprintf(ack.text, "ACK");

                     if(msgsnd(msgid, &ack, sizeof(ack.text), 0) < 0){
                            printf("ACK send failed\n");
                        }
                     else{
                            printf("ACK sent to sync manager\n");
                        }
  

                     write_fifo(m.text);
                }
            }
        return 0;
     }
       
    
