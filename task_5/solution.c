#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>

int out = 0, i = 128;
pid_t pid;

void chldhandler(int signo) {
exit(-1);
}

void prnthandler(int signo) { 
exit(-1);
}
void one(int signo) {
out += i;
i =i/ 2; 
kill(pid, SIGUSR1);
}
void zero(int signo) { 
i=i/2; 
kill(pid, SIGUSR1);
}
void empty(int signo)
{

}

int main(int argc, char ** argv){

pid_t pid1 = getpid(); 

sigset_t set;




struct sigaction act_exit;
memset(&act_exit, 0, sizeof(act_exit));
act_exit.sa_handler = chldhandler; 
sigfillset(&act_exit.sa_mask); 
sigaction(SIGCHLD, &act_exit, NULL); 


struct sigaction act_one;
memset(&act_one, 0, sizeof(act_one));
act_one.sa_handler = one;
sigfillset(&act_one.sa_mask);
sigaction(SIGUSR1, &act_one, NULL);


struct sigaction act_zero;
memset(&act_zero, 0, sizeof(act_zero));
act_zero.sa_handler = zero;
sigfillset(&act_zero.sa_mask); 
sigaction(SIGUSR2, &act_zero, NULL);


sigaddset(&set, SIGUSR1);
sigaddset(&set, SIGUSR2);
sigaddset(&set, SIGCHLD);
sigprocmask(SIG_BLOCK, &set, NULL );
sigemptyset(&set);

pid = fork();


if (pid == 0) {
int reed = 0;
char c = 0;
sigemptyset(&set); 

struct sigaction act_empty; 
memset(&act_empty, 0, sizeof(act_empty));
act_empty.sa_handler = empty;
sigfillset(&act_empty.sa_mask); 
sigaction(SIGUSR1, &act_empty, NULL);
if ((reed = open(argv[1], O_RDONLY)) < 0 ){
exit(-1);
}

int i;

while (read(reed, &c, 1) > 0){ 

for ( i = 128; i >= 1; i /= 2){
if ( i & c ) 
kill(pid1, SIGUSR1);
else 
kill(pid1, SIGUSR2);

sigsuspend(&set); 
} 
}
exit(-1);
}

while(1) { 
if(i == 0){ 

write(STDOUT_FILENO, &out, 1); 
fflush(stdout);

i=128;
out = 0;
}
sigsuspend(&set);
}

exit(-1);
}
