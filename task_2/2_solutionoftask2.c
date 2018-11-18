#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
  char buf[4096], buf1[4096];
  int size;
  
  int fd1[2];
  int fd2[2];
  if (pipe(fd1) < 0) {
    printf("Pipe creation is failed!\n");
    return -1;
  }
  if (pipe(fd2) < 0) {
	printf("Pipe creation is failed!\n");
	return -1;
  }
  const pid_t pid = fork();
  if (pid < 0) {
    printf("fork failed!\n");
    return -1;
  }
  /*there are two pipes #1 and #2 1 if from parent to child 2 is from child to parent*/
  
  
  if (pid) {
    close(fd1[0]);
    close(fd2[1]);   
    while((size = read(0, buf, sizeof(buf)-1)) > 0) 
    {
      buf[size] = 0; // the text string data is expected
      write(fd1[1], buf, size);
      printf("Parent to Child: %s\n", buf);
      size=read(fd2[0],buf1,sizeof(buf1)-1);
      buf1[size] = '\0'; // the text string data is expected
      printf("Parent from Child: %s\n", buf1);
    }

  }
  else 
  {
    close(fd1[1]);
    close(fd2[0]);
    while((size = read(0, buf1, sizeof(buf1)-1)) > 0) 
    {
     
      buf1[size] ='\0'; // the text string data is expected
      write(fd2[1], buf1, size);
      printf("Child to Parent: %s\n", buf1);
      size=read(fd1[0],buf,sizeof(buf)-1);
      buf[size] = '\0'; // the text string data is expected
      printf("Child from Parent: %s\n", buf);

    
    }

  }
  
   
  return 0;
}
