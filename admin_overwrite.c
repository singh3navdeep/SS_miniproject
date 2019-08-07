#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main(int argc,char const *argv[])
{
	struct bank{
        char type;
        char name1[20],name2[20];
        int acc,balance,flag;
        char pwd[9];
        };
	int fd;
	struct bank b;
	fd=open("database",O_RDWR);
	//int c=10;
	//char *name = "akshat";
	//write(fd,&c,sizeof(c));
	//write(fd,name,strlen(name));
	
	//read(fd,&c,sizeof(c));
	//printf("%d%s",c,);
	read(fd,&b,sizeof(b));
	printf("%d\n",b.flag);
	b.flag=0;
	lseek(fd,0,0);
	if(write(fd,&b,sizeof(b))>0)
		printf("\nafterwrite\n");
//	lseek(fd,0,0);
//	if(read(fd,&b,sizeof(b))>0)
  //      	printf("%d%d",b.flag,b.acc);

	//strcpy(b.name1,"Admin");
	//strcpy(b.pwd,"12345678");
	//b.acc=200000;
	//b.balance=0;
	//b.flag=1;
	//strcpy(b.name2,"Admin");
	//write(fd,&b,sizeof(b));
	//lseek(fd,0,0);
	//read(fd,&b,sizeof(b));
	//printf("%d\n",b.flag);
	close(fd);

}

