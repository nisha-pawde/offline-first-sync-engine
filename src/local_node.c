 #include <stdio.h>
 #include <pthread.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include "../include/config.h"
 
 void* data_generator(void* arg){
   
      while(1){
           FILE *fp = fopen(LOCAL_DATA_FILE, "a");
           int value = rand() % 100;
           fprintf(fp, "Data: %d\n", value);
           fclose(fp);
           printf("Generated: %d\n", value);
           sleep(2);
         }
   }

 void* network_monitor(void* arg){
       while(1){
            printf("Checking network...\n");
            sleep(5);
         }
   } 

 int main(){
             pthread_t t1, t2;

             pthread_create(&t1, NULL, data_generator, NULL);
             pthread_create(&t2, NULL, network_monitor, NULL);

              pthread_join(t1, NULL);
              pthread_join(t2, NULL);

              return 0;
    }
