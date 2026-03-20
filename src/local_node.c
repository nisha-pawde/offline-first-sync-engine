

 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>
 #include <sys/shm.h>
 #include <fcntl.h>
 #include "config.h"
 #include <string.h>

 char *shm_ptr;

 void* generate_data(void* arg){
        while(1){
            int data = rand() % 100;
            sprintf(shm_ptr, "DATA:%d", data);
            printf("Generated: %s\n", shm_ptr);
            sleep(2);
          }
     }


 void* write_file(void* arg){
         while(1){
              int fd = open(DATA_FILE, O_CREAT | O_WRONLY | O_APPEND, 0666);
              char buffer[100];
              sprintf(buffer, "%s PENDING\n", shm_ptr);
              write(fd, buffer, strlen(buffer));
              write(fd, "\n", 1);
              close(fd);
              sleep(3);
           }
      }

 int main(){
          int shmid = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT);
          shm_ptr = (char*) shmat(shmid, NULL, 0);

          pthread_t t1, t2;
          pthread_create(&t1, NULL, generate_data, NULL);
          pthread_create(&t2, NULL, write_file, NULL);
          pthread_join(t1, NULL);
          pthread_join(t2, NULL);

          return 0;
       }
