#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<string.h>
int main(int argc, char *argv[])
{
int first[5],server_reply[6],i,choice;
int sockfd, portno, n;
char a[180],b[210];
int pwd;
int acc;
int type;
char msg[500];
int temp;
char name[20],name1[20];
int type1;
int choice1;
int value;
char x;
struct sockaddr_in serv_addr;
if (argc < 3)
{
fprintf(stderr,"usage %s hostname port\n", argv[0]);
exit(0);
}

portno = atoi(argv[2]);
sockfd = socket(AF_INET, SOCK_STREAM, 0);

inet_aton(argv[1], &(serv_addr.sin_addr));

serv_addr.sin_family=AF_INET;

serv_addr.sin_port = htons(portno);
if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
{
        printf("Error in connected\n");
	exit(0);
}
	//printf("connectedtoserver\n");
	read(sockfd,a,sizeof(a));			
	printf("%s",a);
	scanf("%d",&acc);
	write(sockfd,&acc,sizeof(acc));			//sending acc no
	read(sockfd,b,sizeof(b));			
	printf("%s",b);
	scanf("%d",&pwd);
	write(sockfd,&pwd,sizeof(pwd));			//sending password
	
	if(read(sockfd,&type,sizeof(type))<0)		//reading type of acc i.e 'A'=0 , 'N'=1 ,'J'=2
		printf("Reading unsuccessful\n");

		//***** ADMIN AREA******//
	if(type==0)
	{	
		
			
			//printf("Inside a\n");
			if(read(sockfd,msg,sizeof(msg))>0)	// entering choice for 1 add, 2 Logout
			printf("%s\n",msg);
			printf("aftr choice\n");
			scanf("%d",&choice);
			write(sockfd,&choice,sizeof(choice));
			//printf("aftr choice\n");
			if(choice==1)
			{
				
				//printf("inside aftr choce =1\n");
				if(read(sockfd,msg,sizeof(msg))<0)
					printf("unread success\n");	
				printf("%s\n",msg);			//msg=type of account
				scanf(" %d",&type1);
				if(write(sockfd,&type1,sizeof(type1))<0)
					printf("write of type is unsuccess\n");
				if(type1==3)
				{	//printf("detail of normal\n");
					read(sockfd,msg,sizeof(msg));	
					printf("%s\n",msg);
					scanf("%s",name);
					write(sockfd,name,sizeof(name));
				}
				else
				{
					read(sockfd,msg,sizeof(msg));	
					printf("%s\n",msg);
					scanf("%s",name);
					write(sockfd,name,sizeof(name));
					read(sockfd,msg,sizeof(msg));	
					printf("%s\n",msg);
					scanf("%s",name);
					write(sockfd,name,sizeof(name));
				}
				read(sockfd,msg,sizeof(msg));			//server demand for pwd
				printf("%s\n",msg);
				scanf("%d",&pwd);
				write(sockfd,&pwd,sizeof(pwd));
				read(sockfd,msg,sizeof(msg));			//success msg
				printf("%s\n",msg);	
			}//closing if choice==1
			else						//Logout portion i.e choice =2
			{
				read(sockfd,msg,sizeof(msg));		
				printf("%s\n",msg);
				exit(0);
			}
		
	}//close if of type==0
	else
	if(type==1)
	{
		while(1)
		{
			//printf("inside user");
			if(read(sockfd,msg,sizeof(msg))>0)	// entering choice msg frm server
		        printf("%s\n",msg);
			scanf("%d",&choice1);
			if(write(sockfd,&choice1,sizeof(choice1))<0)			
			printf("choice is not send to server %d\n",choice1);

			switch(choice1)
			{	//printf("inside switch\n");
				case 1:
				{	//printf("case 1\n");
					if(read(sockfd,msg,sizeof(msg))>0)	
					printf("%s\n",msg);
					scanf("%d",&value);			//amount to be deposited
					write(sockfd,&value,sizeof(value));
					if(read(sockfd,msg,sizeof(msg))>0)	//success message from server	
					printf("%s\n",msg);
				}break;
				
				case 2:
				{	//printf("case 2\n");
					if(read(sockfd,msg,sizeof(msg))>0)	
					printf("%s\n",msg);
					scanf("%d",&value);			//amount to withdraw
					write(sockfd,&value,sizeof(value));
					if(read(sockfd,msg,sizeof(msg))>0)	//message from server
					printf("%s\n",msg);
				}break;
				
				case 3:
				{	//printf("case 3\n");
					//if(read(sockfd,msg,sizeof(msg))>0)	
					printf("Your Current Balance Is::\n");
					if(read(sockfd,&value,sizeof(value))>0)	
					printf("%d\n",value);
				}break;
		
				case 4:
				{	//printf("case 4\n");
					//if(read(sockfd,msg,sizeof(msg))>0)	
					printf("Your Account Details Are:: \n");
					//printf("after message\n");
					if(read(sockfd,&value,sizeof(value))>0)		//receiving account number
					printf("Your account no is : %d\n",value);
					if(read(sockfd,&x,sizeof(x))>0)			//receiving Type
					printf("Your account type is : %c\n",x);
					if(read(sockfd,name,sizeof(name))>0)		//receiving name
					printf("Account member name : %s\n",name);
					if(x=='J')
					if(read(sockfd,name1,sizeof(name1))>0)		//receiving joint name
					printf("Account joint member name : %s\n",name1);
				}break;

				case 5:
				{	//printf("case 5\n");
					if(read(sockfd,msg,sizeof(msg))>0)	
					printf("%s\n",msg);
					scanf("%d",&pwd);
					write(sockfd,&pwd,sizeof(pwd));
					if(read(sockfd,msg,sizeof(msg))>0)	
					printf("%s\n",msg);
				}break;

				case 6:
				{	//printf("case 6\n");
					if(read(sockfd,msg,sizeof(msg))>0)	
					printf("%s\n",msg);
					exit(0);
				}
			}//switch close
		}//while close

	}//if type=1 closed
	else
	{
		//printf("error occured in type= 2");
		read(sockfd,msg,sizeof(msg));
		printf("%s\n",msg);
	}
}









