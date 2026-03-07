
 #ifndef IPC_H
 #define IPC_H

 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/msg.h>
 #include <sys/shm.h>
 #include <sys/sem.h>
 #include <unistd.h>
 
 #define PIPE_READ 0
 #define PIPE_WRITE 1
 
 #define MSG_QUEUE_KEY 1234
 #define SHM_KEY 5678
 #define SEM_KEY 9101

 struct message {

        long type;
        char text[256];

   };

  #endif
