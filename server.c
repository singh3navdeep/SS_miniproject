#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include <arpa/inet.h>
#include<string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>

					//??******DECELERATIONS******??//
int nextaccount=206001;

struct bank{
	char type;
	char name1[20],name2[20];
	int acc,balance,flag;
	//char pwd[9];
	int pwd;
	};



					//??****** ADMIN AREA ******??//

void admin(int newsockfd,int fd)
{
	printf("\nInsideAdmin\n");
	int choice,x=0;
	struct bank b,c2;
	int type;
	char name1[20],name2[20];
	//char pass[8];
	int pass;
	lseek(fd,0,0);
	read(fd,&b,sizeof(b));
	printf("value of flag%d%d\n",b.flag,b.acc);
	b.flag=b.flag-1;
	lseek(fd,0,0);
	if(write(fd,&b,sizeof(b))>0){
		fdatasync(fd);
		printf("\nchecking\n");
	}
			//**REPEATED MENU FOR ADMIN**//
	do{
		write(newsockfd,"1-Add User\n2-Logout\nEnter Your choice::\n",sizeof("1-Add User\n2-Logout\nEnter Your choice::\n"));
		read(newsockfd,&choice,sizeof(choice));
		if(choice==1)				//REPRESENTING ADMIN WANTS TO ADD A NEW USER
		{
			lseek(fd,0,SEEK_END);
			write(newsockfd,"Enter Type of account \nfor normal press 3\nfor joint press 4 : \n",sizeof("Enter Type of account \nfor normal press 3\n forr joint press 4 : \n"));
			//printf("write run\n");
			//printf("hiiiiiiii\n");
			//printf("%d\n",type);
			fdatasync(fd);
			if(read(newsockfd,&type,sizeof(type))>0)
			printf("READ IS WORKING AND VALUE OF TYPE IS : %d\n",type);
			printf("%d",type);
			if(type==3)			//REPRESENTING NORMAL ACCOUNT
			{
				write(newsockfd,"Enter Your Name::\n",sizeof("Enter Your Name::\n"));
				read(newsockfd,name1,sizeof(name1));
				printf("name is entered successfully\n name is %s \n",name1);
				b.flag=1;
				strcpy(b.name1,name1);
				strcpy(b.name2,"NULL");
				printf("successful completion of block 3\n");
			}
			else if(type==4)		//REPRESENTING JOINT ACCOUNT
			{
				write(newsockfd,"Enter Your Name1::\n",sizeof("Enter Your Name1::\n"));
                                read(newsockfd,name1,sizeof(name1));
				printf("name1 is entered successfully\n name1 is %s \n",name1);
				write(newsockfd,"Enter Your Name2::\n",sizeof("Enter Your Name2::\n"));
                                read(newsockfd,name2,sizeof(name2));
				printf("name2 is entered successfully\n name2 is %s \n",name2);
				b.flag=2;
				strcpy(b.name1,name1);
	                        strcpy(b.name2,name2);
				printf("block 2 is over");

			}
			write(newsockfd,"Enter your 8 digit password::\n",sizeof("Enter your 8 digit password::\n"));
			read(newsockfd,&pass,sizeof(pass));
			printf("password is readed successful\n and pass is = %d\n",pass);
			if(type==3)
			b.type='N';
			else
			b.type='J';
			lseek(fd,-1*sizeof(c2),SEEK_END);
			read(fd,&c2,sizeof(c2));
			printf("%d\n",c2.acc);
			b.acc=c2.acc+1;
			b.pwd=pass;
			b.balance=0;
			write(fd,&b,sizeof(b));
			write(newsockfd,"User info is correctly saved\n",sizeof("User info is correctly saved\n"));
			
			lseek(fd,0,SEEK_SET);
			read(fd,&b,sizeof(b));
			b.flag=1;
			lseek(fd,0,SEEK_SET);
			write(fd,&b,sizeof(b));
		}

		else				
		{	
			b.flag=1;
			lseek(fd,0,SEEK_SET);
			write(fd,&b,sizeof(b));
			write(newsockfd,"You are successfully loged out\n",sizeof("You are successfully loged out\n"));
			close(fd);
			exit(0);
		}

	}while(1);

}

// ??***** USER AREA *****??//

void deposit(int newsockfd,int fd)
{
	printf("Insise deposit function\n");
	struct bank b;
	int amount;
	lseek(fd,-1*sizeof(b),SEEK_CUR);
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(b);
	if(fcntl(fd,F_SETLKW,&lock)==-1)
	{
		printf("Error in locking.");
	}
	else
	{
		read(fd,&b,sizeof(b));
		write(newsockfd,"Enter the amount to deposit :: \n",sizeof("Enter the amount to deposit :: \n"));
		read(newsockfd,&amount,sizeof(amount));
		printf("new amount is readed from server\n");
		b.balance = b.balance + amount;
		lseek(fd,-1*sizeof(b),SEEK_CUR);
		if(write(fd,&b,sizeof(b))>0)
		{
			fdatasync(fd);
			printf("amount is deposited\n");
		}
		write(newsockfd,"Amounnt is deposited successfully \n",sizeof("Amounnt is deposited successfully \n"));
		lock.l_type=F_UNLCK;
		lseek(fd,-1*sizeof(b),SEEK_CUR);
		if(fcntl(fd,F_SETLKW,&lock)==-1)
		{
			printf("Error in unlocking.\n");
		}
		printf("unlocked frm deposits \n");
		lseek(fd,sizeof(b),SEEK_CUR);
		return;

	}
}

void withdraw(int newsockfd,int fd)
{

	struct bank b;
	int amount;
	lseek(fd,-1*sizeof(b),SEEK_CUR);
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(b);
	if(fcntl(fd,F_SETLKW,&lock)==-1)
	{
		printf("Error in locking.");
	}
	else
	{
		read(fd,&b,sizeof(b));
		write(newsockfd,"Enter the amount to withdraw :: \n",sizeof("Enter the amount to withdraw :: \n"));
		read(newsockfd,&amount,sizeof(amount));
		printf("withdrawing amount is readed  server\n");
		if(b.balance>=amount)
		{
			b.balance=b.balance-amount;
			lseek(fd,-1*sizeof(b),SEEK_CUR);
			if(write(fd,&b,sizeof(b))>0)
			{
				fdatasync(fd);
				printf("amount is withdrawn\n");
				write(newsockfd,"Amounnt is Withdrswn successfully \n",sizeof("Amounnt is Withdrawn successfully \n"));
			}
			else
			{
				printf("Error in updating balance in database");
				write(newsockfd,"Unable to withdraw money \n",sizeof("Unable to withdraw money \n"));
			}
			
		}
		else
		{
			write(newsockfd,"Insufficient funds\n",sizeof("Insufficient funds\n"));
		}
		lock.l_type=F_UNLCK;				//UNLOCKING 
		lseek(fd,-1*sizeof(b),SEEK_CUR);
		if(fcntl(fd,F_SETLKW,&lock)==-1)
		{
			printf("Error in unlocking.\n");
		}
		printf("unlocked frm WITHDRAW \n");
		lseek(fd,sizeof(b),SEEK_CUR);
		return;

	}
}

void balance_enquery(int newsockfd,int fd)
{
	struct bank b;
	lseek(fd,-1*sizeof(b),SEEK_CUR);
	//struct flock lock;
	//lock.l_type = F_RDLCK;
	//lock.l_whence = SEEK_CUR;
	//lock.l_len = sizeof(b);
	//if(fcntl(fd,F_SETLKW,&lock)==-1)
	//{
	//	printf("Error in locking.");
	//}
	//else
	//{
		read(fd,&b,sizeof(b));
		//write(newsockfd,"Current Balance is :: \n",sizeof("Current Balance is :: \n"));
		write(newsockfd,&b.balance,sizeof(int));
		printf("balance enquery has completed unlck remaining");
	//}
	
		//lock.l_type=F_UNLCK;				//UNLOCKING 
		//lseek(fd,-1*sizeof(b),SEEK_CUR);		
		//if(fcntl(fd,F_SETLKW,&lock)==-1)
		//{
		//	printf("Error in unlocking.\n");
		//}
		//printf("unlocked frm Balance Enquery \n");
		//lseek(fd,sizeof(b),SEEK_CUR);
		return;

}

void details(int newsockfd,int fd)
{
	struct bank b;
	lseek(fd,-1*sizeof(b),SEEK_CUR);
	read(fd,&b,sizeof(b));
	printf("helllouser%d\n",b.acc);
	//write(newsockfd,"Your account details are :: \n",sizeof("Your account details are :: \n"));
	if(write(newsockfd,&b.acc,sizeof(b.acc))>0)
		printf("writing account number to client\n");
	write(newsockfd,&b.type,sizeof(char));
	write(newsockfd,b.name1,sizeof(b.name1));
	if(b.type=='J')
	write(newsockfd,b.name2,sizeof(b.name2));
	printf("details shown to the client");
	return;

}

void change_password(int newsockfd,int fd)
{
	struct bank b;
	int password;
	lseek(fd,-1*sizeof(b),SEEK_CUR);
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(b);
	if(fcntl(fd,F_SETLKW,&lock)==-1)
	{
		printf("Error in locking.");
	}
	else
	{
		read(fd,&b,sizeof(b));
		write(newsockfd,"Enter the new password :: \n",sizeof("Enter the new password :: \n"));
		read(newsockfd,&password,sizeof(password));
		printf("new new password is readed by server\n");
		b.pwd=password;
		lseek(fd,-1*sizeof(b),SEEK_CUR);
		if(write(fd,&b,sizeof(b))>0)
		{
			fdatasync(fd);
			printf("password is changed\n");
		}
		write(newsockfd,"Password is changed successfully \n",sizeof("Password is changed successfully \n"));
		lock.l_type=F_UNLCK;
		lseek(fd,-1*sizeof(b),SEEK_CUR);	
		if(fcntl(fd,F_SETLKW,&lock)==-1)
		{
			printf("Error in unlocking.\n");
		}
		printf("unlocked frm change password \n");
		lseek(fd,sizeof(b),SEEK_CUR);
		return;

	}
}

void logout(int newsockfd,int fd)
{
	printf("inside logout function\n");
	struct bank b;
	lseek(fd,-1*sizeof(b),SEEK_CUR);
	/*struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_CUR;
	lock.l_len = sizeof(b);
	if(fcntl(fd,F_SETLKW,lock)==-1)
	{
		printf("Error in locking.");
	}
	else
	{*/
		read(fd,&b,sizeof(b));
		printf("at server print accno in logout%d\n",b.acc);
		b.flag=b.flag+1;
		lseek(fd,-1*sizeof(b),SEEK_CUR);
		if(write(fd,&b,sizeof(b))>0)
		{
			fdatasync(fd);
			printf("Logging out is \n");
		}
		write(newsockfd,"successfully signout\n",sizeof("successfully signout\n"));
		//lock.l_type=F_UNLCK;
		//if(fcntl(fd,F_SETLKW,lock)==-1)
		//{
		//	printf("Error in unlocking.\n");
		//}
		//printf("unlocked frm logout \n");
		close(fd);
		close(newsockfd);
		exit(0);
	//}

}	

void user(int newsockfd,int fd)				//**USER**//
{
printf("\nInside User\n");
	struct bank b;
	int choice;
	lseek(fd,-1*sizeof(b),SEEK_CUR);
	read(fd,&b,sizeof(b));
	printf("value of flag %d \nand acc = %d\n",b.flag,b.acc);
	b.flag=b.flag-1;
	lseek(fd,-1*sizeof(b),SEEK_CUR);
	if(write(fd,&b,sizeof(b))>0)
	{
		fdatasync(fd);
		printf("flag is updated\n");
	}
	do
	{
		write(newsockfd,"Welcome\n1-DEPOSIT User\n2-WITHDRAW\n3-BALANCE ENQUERY\n4-DETAILS\n5-CHANGE PASSWORD\n6-LOGOUT\nEnter Your choice::\n",sizeof("Welcome\n1-DEPOSIT User\n2-WITHDRAW\n3-BALANCE ENQUERY\n4-DETAILS\n5-CHANGE PASSWORD\n6-LOGOUT\nEnter Your choice::\n"));
		read(newsockfd,&choice,sizeof(choice));
		printf("readed choice successfull");
		
		
			switch(choice)
			{
				case 1: {deposit(newsockfd,fd); break;}
				case 2: {withdraw(newsockfd,fd); break;}
				case 3: {balance_enquery(newsockfd,fd); break;}
				case 4: {details(newsockfd,fd);break;}
				case 5: {change_password(newsockfd,fd);break;}
				case 6: {logout(newsockfd,fd);break;}
				default: printf("please enter a valid number");
			}
	}while(1);



}


void login(int newsockfd){
		struct bank b;
                //char passwd[9];
		int passwd;
		int u_id,x=0;
		int fd;
                fd=open("database",O_RDWR);
                write(newsockfd,"Enter Your UserId::\n",sizeof("Enter Your UserId::\n"));
                read(newsockfd,&u_id,sizeof(u_id));
		write(newsockfd,"Enter Your Password::\n",sizeof("Enter Your Password::\n"));
		read(newsockfd,&passwd,sizeof(passwd));
		printf("%d\n",u_id);
		printf("%d\n",passwd);
                while(read(fd,&b,sizeof(b))>0)
		{
			printf("\nInside while of server to check\n");
			printf("%d\t%d\n",b.acc,b.pwd);
			if(b.acc==u_id && b.pwd==passwd)
			{
				printf("inside if inside while%d\n",b.acc);
			
				if(b.flag>0)
				{
					if(b.type=='A')
					{
						if(write(newsockfd,&x,sizeof(int))>0)
							printf("aftr write");
						printf("\ncalling admin\n");
						admin(newsockfd,fd);
					}
					else
					{
						x=1;
						write(newsockfd,&x,sizeof(int));
						user(newsockfd,fd);
					}

				}
				else
				{
					x=2;
					write(newsockfd,&x,sizeof(int));
					write(newsockfd,"Already Login,Please Try later::\0",sizeof("Already Login,Please Try later::\0"));
					exit(0);
				}
			}

		}//close while
		x=2;
		write(newsockfd,&x,sizeof(int));
		write(newsockfd,"Wrong UserName And Password\0",sizeof("Wrong UserName And Password\0"));
}



int main(int argc,char *argv[])
{
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	if (argc < 2)
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		printf("ERROR opening socket");
		exit(1);
	}


	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;		//AF_INET refers to addresses from the internet, IP addresses specifically.
	serv_addr.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY can binds to all IP. Not for specific IP.
	serv_addr.sin_port = htons(portno);     //The htons function can be used to convert an IP port number in host byte order to the IP port number in network byte order.
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
		printf("ERROR on binding\n");
		exit(1);
	}

	printf("Waiting for Client\n");

	listen(sockfd,5);
	printf("after listen\n");
	clilen = sizeof(cli_addr);

	while(1)
	{
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		printf("after accept\n");
		if (newsockfd < 0)
		{
		        printf("ERROR on accept");
		}
		if(!fork())
		{
			printf("going to login\n");
			login(newsockfd);
		}
	}
	
}

