
 #include <stdio.h>
 #include <sys/msg.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include "config.h"
 #include "ipc.h"
 #include <string.h>

 struct msg{
        long type;
        char text[100];
     };

 int main(){

        int msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
        struct msg m;

        create_fifo();

        while(1){
              msgrcv(msgid, &m, sizeof(m), MSG_DATA, 0);
              printf("Server received: %s\n", m.text);
  
              int fd = open(CENTRAL_FILE, O_CREAT | O_WRONLY | O_APPEND, 0666);
              write(fd, m.text, strlen(m.text));
              write(fd, "\n", 1);
              close(fd);

              struct msg ack;
              ack.type = MSG_ACK;
              sprintf(ack.text, "ACK");
              msgsnd(msgid, &ack, sizeof(ack), 0);  

              write_fifo(m.text);
           }

        return 0;
       
       }
