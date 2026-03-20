
 #include <stdio.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include "ipc.h"
 #include "config.h"
 #include <string.h>

 int main(){

         char buffer[100];

         while(1){
              read_fifo(buffer);
              printf("Logger received: %s\n", buffer);

              int fd = open(LOG_FILE, O_CREAT | O_WRONLY | O_APPEND, 0666);
              write(fd, buffer, strlen(buffer));
              write(fd, "\n", 1);
              lseek(fd, 0, SEEK_END);
 
              close(fd);
            }
         return 0;
    }
