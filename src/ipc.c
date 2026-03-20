 #include "ipc.h"
 #include "config.h"
 #include <fcntl.h>
 #include <unistd.h>
 #include <sys/stat.h>
 #include <stdio.h>


 void create_fifo() {
        mkfifo(FIFO_NAME, 0666);
     }

 void write_fifo(const char *msg) {
        int fd = open(FIFO_NAME, O_WRONLY);
        write(fd, msg, 100);
        close(fd);
     }

 void read_fifo(char *buffer) {
        int fd = open(FIFO_NAME, O_RDONLY);
        read(fd, buffer, 100);
        close(fd);
     }
