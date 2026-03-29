
 #include <stdlib.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <signal.h>

 pid_t p1, p2, p3, p4;

 void handler(int sig){

       if(sig == SIGINT){
                printf("\nShutting down system...\n");

                kill(p1, SIGTERM);
                kill(p2, SIGTERM);
                kill(p3, SIGTERM);
                kill(p4, SIGTERM);

                exit(0);
           }

        else if(sig == SIGTERM){
                printf("Force stopping all processes\n");
 
                kill(p1, SIGKILL);
                kill(p2, SIGKILL);
                kill(p3, SIGKILL);
                kill(p4, SIGKILL);

                exit(0);
           }

         else if(sig == SIGCHLD){
                printf("Child process ended\n");
                while(waitpid(-1, NULL, WNOHANG) > 0);
           }

         else if(sig == SIGUSR1){
                printf("Manual signal received (can trigger sync)\n");
            }
          }

 int main(){

          signal(SIGINT, handler);
          signal(SIGCHLD, handler);
          signal(SIGUSR1, handler);

          p1 = fork();
          if(p1 == 0){
                execl("./local_node", "local_node", NULL);
                perror("exec local_node failed");
                exit(1);
              }

          p2 = fork();  
          if(p2 == 0){
                execl("./sync_manager", "sync_manager", NULL);
                perror("exec sync_manager failed");
                exit(1);
              }

          p3 = fork();
          if(p3 == 0){
                execl("./central_server", "central_server", NULL);
                perror("exec central_server failed");
                exit(1);
              }

          p4 = fork();
          if(p4 == 0){
                execl("./logger", "logger", NULL);
                perror("exec logger failed");
                exit(1);
              }

          while (wait(NULL) > 0);

          return 0;
     }

