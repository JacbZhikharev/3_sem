/* Программа, осуществляющая однонаправленную связь через pipe между процессом-родителем и процессом-ребенком */

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
int fd1[2],fd2[2];
pid_t result;
size_t size;
char resstring[30],resstring1[30];
char read1[30];
char read2[30];

/* Попытаемся создать pipe */

if(pipe(fd1) < 0){

	/* Если создать pipe не удалось, печатаем об этом сообщение и прекращаем работу */

	printf("Can\'t create pipe\n");
	exit(-1);
}
	/* Аналогично для второго pip'а*/
if (pipe(fd2) < 0){
	printf("Can\'t create pipe\n");
	exit(-1);
}
	/* Порождаем новый процесс */
scanf("%s",read1);
scanf("%s",read2);

result = fork();
if(result <0){

	/* Если создать процесс не удалось, сообщаем об этом и завершаем работу */

	printf("Can\'t fork child\n");
	exit(-1);

}
else if (result > 0) {

	/* Мы находимся в родительском процессе, который будет передавать информацию процессу-ребенку . В этом процессе выходной поток данных нам не понадобится, поэтому закрываем его.*/
	
	close(fd1[0]);

	/* Пробуем записать в pipe 14 байт, т.е. всю строку "Hello, world!"
	вместе с признаком конца строки */

	size=write(fd1[1], read1, strlen(read1)+1);
	if(size < 0)
        {
                printf("Can\'t write string\n");
                exit(-1);
        }

	/* Закрываем входной поток данных и на этом родитель прекращает работу */
	close(fd1[1]);
	close(fd2[2]);
	size=read(fd2[0],resstring1, strlen(read2)+1);
	if(size < 0)
	{
                printf("Can\'t read string\n");
                exit(-1);
        }
	printf("%s That Parent get from Child\n",resstring1);
	close(fd2[0]);


} else {

	/* Мы находимся в порожденном процессе, который будет получать информацию от процесса-родителя. Он унаследовал от родителя таблицу открытых файлов и, зная файловые дескрипторы, соответствующие pip'у, может его использовать. В этом процессе входной поток данных нам не понадобится, поэтому закрываем его.*/
	
	close(fd1[1]);

	size = read(fd1[0], resstring, strlen(read1)+1);

	if(size < 0){

		/* Если прочитать не смогли, сообщаем об ошибке и завершаем работу */

		printf("Can\'t read string\n");
		exit(-1);
	}	 

	/* Печатаем прочитанную строку */

	printf("%s That Child get from Parent\n",resstring);

	/* Закрываем входной поток и завершаем работу */

	close(fd1[0]);
	close(fd2[0]);
	size=write(fd2[1],read2,strlen(read2)+1);
	if(size < 0)
        {
                printf("Can\'t write string\n");
                exit(-1);
        }

	close(fd2[1]);

	}

return 0;
} 
