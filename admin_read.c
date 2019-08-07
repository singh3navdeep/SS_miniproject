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
	int pwd;
        };
	int fd;
	struct bank b;
	fd=open("database",O_RDONLY);
	//b.type='A';
//	strcpy(b.name1,"Admin");
//	strcpy(b.pwd,"12345678");
//	b.acc=200000;
//	b.balance=0;
//	b.flag=1;
//	strcpy(b.name2,"Admin");
	//write(fd,&b,sizeof(b));
	while(read(fd,&b,sizeof(b))>0){
		//printf("inside while");
		printf("%d\t%d\t%s\t%c\t%d\t%d\n",b.flag,b.acc,b.name1,b.type,b.pwd,b.balance);
}
	close(fd);

}

