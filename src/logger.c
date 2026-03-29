
 #include <stdio.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include "ipc.h"
 #include "config.h"
 #include <string.h>
 #include <stdlib.h>

 int main(){

         char buffer[100];

         while(1){
              memset(buffer, 0, sizeof(buffer));
              read_fifo(buffer);

              printf("Logger received: %s\n", buffer);

              int fd = open(LOG_FILE, O_CREAT | O_WRONLY | O_APPEND, 0666);

              if(fd >= 0){
                    lseek(fd, 0, SEEK_END);
                    write(fd, buffer, strlen(buffer));
                    write(fd, "\n", 1);
             
                    close(fd);
                 }
              else{
                   printf("Log file open failed\n");
                 }

            }

         return 0;
    }
