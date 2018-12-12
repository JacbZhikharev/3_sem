#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
static void run_cmd(char *cmd)
{
        int i=0;
        const pid_t pid = fork();
        if (pid < 0) {
        printf("ForkFailed!");
        }
        if (pid)
        {
                int status;
                waitpid(pid,&status, 0);
                printf("Rcode: %d",WEXITSTATUS(status));

        }
        char ** args;
        args=malloc(200*sizeof(char));        
        char delim[]=" , \n";
        for (char *p= strtok(cmd,delim); p!=NULL; p = strtok(NULL, delim))
        {
        *(args+i)=p;
        i++;

        }


        execvp(args[0],args);
        printf("111");
        free(args);
}
int main()
{


        while(1)
        {
                char *cmd;
                cmd = malloc(200*sizeof(char));
                fgets(cmd,200,stdin);
        //      printf("qwerty");
                run_cmd(cmd);
                free(cmd);
        }
return 0;

}
      
