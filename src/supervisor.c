
 #include <stdlib.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <signal.h>

 void handler(int sig){
          if(sig == SIGINT){
                  printf("\nShutting down system...\n");
            }
          else if(sig == SIGCHLD){
                     printf("Child process ended\n");
                  while(waitpid(-1, NULL, WNOHANG) > 0);
                  }
          else if(sig == SIGUSR1) {
                     printf("Manual signal received\n");
                  }
      }

 int main(){

          signal(SIGINT, handler);
          signal(SIGCHLD, handler);
          signal(SIGUSR1, handler);

          if(fork() == 0){
                execl("./local_node", "local_node", NULL);
                perror("exec local_node failed");
                exit(1);
              }

          if(fork() == 0){
                execl("./sync_manager", "sync_manager", NULL);
                perror("exec sync_manager failed");
                exit(1);
              }

          if(fork() == 0){
                execl("./central_server", "central_server", NULL);
                perror("exec central_server failed");
                exit(1);
              }

          if(fork() == 0){
                execl("./logger", "logger", NULL);
                perror("exec logger failed");
                exit(1);
              }

          while (wait(NULL) > 0);

          return 0;
     }

