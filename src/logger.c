
 #include <stdio.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include "../include/config.h"

 int main(){

      int fd;
      
      fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
       
      if(fd < 0) {
         printf("Error opening log file\n");
         return 1;
       }

       write(fd, "Logger started\n", 15);

       close(fd);

 }
