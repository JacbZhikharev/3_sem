#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

int n; 
#define N 100000000 
int main(){
	printf("STREAMS\n");
	scanf("%d", &n);

	int * array;
	int i;
	int shmid;
	char pathname[] = "integral.c";
	key_t key;
	if ((key = ftok(pathname,0)) < 0) {
		exit(-1);
	}
	if ((shmid = shmget(key, n*sizeof(int), 0)) < 0) {
			exit(-1);
	}
	if ((array = (int*)shmat(shmid, NULL, 0)) == (int*) (-1)) {
		exit(-1);
	}

	int sum = 0;
	for (i = 0; i < n; i++) {
		sum += array[i];
	}
	if (shmdt(array) < 0) {
		exit(-1);
	}
	int status = shmctl(shmid, IPC_RMID, NULL);
	printf("%f\n", (double)sum * 3  / N);
	return 0;
}
