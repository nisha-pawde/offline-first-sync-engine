 #include "ipc.h"
 #include "config.h"
 #include <stdio.h>
 #include <fcntl.h>
 #include <unistd.h>
 #include <sys/stat.h>

 void create_fifo() {
    mkfifo(FIFO_NAME, 0666);
    printf("FIFO created\n");
   }
