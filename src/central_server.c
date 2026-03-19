
 #include <stdio.h>
 #include <unistd.h>
 #include "../include/config.h"
 
 int main(){
        printf("Central Server Started\n");

        while(1){
             FILE *fp = fopen(CENTRAL_DATA_FILE, "a");
             fprintf(fp, "Central Data Updated\n");
             fclose(fp);
             printf("Central Server: Data Stored\n");
             sleep(5);
           }
        return 0;
     }
