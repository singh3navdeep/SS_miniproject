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
        //char pwd[9];
	int pswd;
        };
	int fd;
	struct bank b;
	fd=open("database",O_CREAT|O_RDWR|O_APPEND,0777);
	b.type='A';
	strcpy(b.name1,"Admin");
	b.pswd=12345678;
	b.acc=200000;
	b.balance=0;
	b.flag=1;
	strcpy(b.name2,"NULL");
	write(fd,&b,sizeof(b));
	close(fd);

}

