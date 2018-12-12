#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define N 100000000 


int *array;
int n=1; //число ядер

typedef struct {
	int num;
} input_t;


void * generate(void * d);


int main() {
	
	int i;
	/*printf("STREAM\n");
	scanf("%d", &n);*/
	pthread_t *thread = (pthread_t *) malloc (n * sizeof(pthread_t));
	srand(time(NULL));
	input_t *t = (input_t *) malloc (n * sizeof(input_t));

	int shmid;
	char pathname[] = "integral.c";
	key_t key;
	if ((key = ftok(pathname,0)) < 0) 
  {
		exit(-1);
	}
	if ((shmid = shmget(key, n * sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) 
  {
		if (errno != EEXIST) 
    {
			exit(-1);
		} 
    else 
    {
		if ((shmid = shmget(key, n * sizeof(int), 0)) < 0) 
      {
			exit(-1);
			}
		}
	}
	array = (int*)shmat(shmid, NULL, 0);
	if (array == (int*) (-1)) 
  {
		exit(-1);
	}

	for (i = 0; i < n; i++) 
  {
		array[i] = 0;
	}



	for (i = 0; i < n; i++) 
  {
		t[i].num = i;
		int st =pthread_create (&thread[i], (pthread_attr_t * )NULL, generate, (void*)&t[i]);
		if (st != 0) 
    {
			exit(-1);
		}
	}
	
	for (i = 0; i < n; i++) 
  {
		pthread_join(thread[i], (void **)NULL);
	}
	if (shmdt(array) < 0) 
  {
		exit(-1);
	}
	free(thread);
	free(t);
	return 0;
}

void * generate(void * d) {
	double x, y, y1;
	long long j, jmax;
	unsigned int seed = time(NULL);
	input_t * data = (input_t *)d;
	int i = data->num;
	if (i != 0) 
  {
		jmax = N / n;
	} else 
  {
		jmax = (N / n) + (N % n);
	}
	int k = 0;
	for (j = 0; j < jmax ; j++) 
  {
		x = (double)(rand_r(&seed)) / RAND_MAX;
		x = 2 * x;
		y = (double)(rand_r(&seed))/ RAND_MAX;
		y = 4*y;
		y1 = x * x;
		if (y <= y1) 
    {
			k ++;
		}
	}
	array[i] = k;
	
	return 0;
}
