#include	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<unistd.h>
#include	<sys/wait.h>
#include	<pthread.h>


#define BUFSIZE 512
#define MAXSERVERNAME 100
#define port 11710
#define MYPORT 11710
#define MAXNAME 100 
#define h1ip "10.0.0.1"
#define h2ip "10.0.10.2"
#define h3ip "10.0.11.3"
#define h4ip "10.0.0.5"

typedef struct listen_sock
{
    int fd;
    struct sockaddr_in addr_my;
    
}listen_sock;

typedef struct dest_sock
{
    int sd;
    struct	sockaddr_in serveraddress1;
    struct	sockaddr_in serveraddress2;
    struct	sockaddr_in serveraddress3;
    struct	sockaddr_in serveraddress4;
    
}dest_sock;

void* my_work( void *);
void* receiver(void* );


int main( int C, char *argv[] )
{
	dest_sock destsock = {-1};
	listen_sock msock = {-1};
	int ret;
	
	
	destsock.sd = socket( AF_INET, SOCK_DGRAM, 0 );
	if(0 > destsock.sd ) 
	{
		perror( "socket" );
		exit (1);
	}
        
	memset( &destsock.serveraddress1, 0, sizeof(destsock.serveraddress1) );
	destsock.serveraddress1.sin_family = AF_INET;
	destsock.serveraddress1.sin_port = htons(port);
	destsock.serveraddress1.sin_addr.s_addr = inet_addr(h1ip);
memset( &destsock.serveraddress2, 0, sizeof(destsock.serveraddress2) );
	destsock.serveraddress2.sin_family = AF_INET;
	destsock.serveraddress2.sin_port = htons(port);
	destsock.serveraddress2.sin_addr.s_addr = inet_addr(h2ip);
memset( &destsock.serveraddress3, 0, sizeof(destsock.serveraddress3) );
	destsock.serveraddress3.sin_family = AF_INET;
	destsock.serveraddress3.sin_port = htons(port);
	destsock.serveraddress3.sin_addr.s_addr = inet_addr(h3ip);
memset( &destsock.serveraddress4, 0, sizeof(destsock.serveraddress4) );
	destsock.serveraddress4.sin_family = AF_INET;
	destsock.serveraddress4.sin_port = htons(port);
	destsock.serveraddress4.sin_addr.s_addr = inet_addr(h4ip);
	
	printf("Client Starting service\n");

    	listen_sock *sock1 = &msock;
   	dest_sock *sock2 = &destsock; 
    
	
	
 
    	pthread_t t1,t2;
   	pthread_create(&t2, NULL, my_work, (void *)sock2);
  	pthread_create(&t1, NULL, receiver, (void *)sock1);
 
  	pthread_join(t1,NULL);
   	pthread_join(t2,NULL);


	
	exit(0);
}


void* my_work(void *sockinfo)	/*Connection Socket */
{
	FILE *fp=stdin;
	struct dest_sock *ds=(dest_sock *)sockinfo;
	int sockfd =ds->sd;
	struct sockaddr *to1 =(struct sockaddr *)&(ds->serveraddress1);
	struct sockaddr *to2 =(struct sockaddr *)&(ds->serveraddress2);
	struct sockaddr *to3 =(struct sockaddr *)&(ds->serveraddress3);
	struct sockaddr *to4 =(struct sockaddr *)&(ds->serveraddress4);
	socklen_t length=sizeof(ds->serveraddress4);

	char sendbuf[BUFSIZE], recvbuf[BUFSIZE],servername[MAXSERVERNAME];
	char *cptr;
	int ret, numbytes, slen;
	socklen_t structlen;
	struct sockaddr_in serveraddr;
	
	for(;;)
	{
		
		printf("Enter Data or press ctl-c to exit:\n");
		/*Reading data from the keyboard*/
		cptr = fgets(sendbuf,BUFSIZE,fp);
		
		if (NULL == cptr)
		{
			printf("Possible error or end of file\n");
		
		}
		slen = strlen (sendbuf);
		/*Sending the read data over socket*/
		ret = sendto(sockfd,sendbuf,slen,0,to4,length);
		if (0 > ret)
		{
			perror("Error in sending data-h4:\n");
			
		}

		
		/*Sending the read data over socket*/
		ret = sendto(sockfd,sendbuf,slen,0,to3,length);
		if (0 > ret)
		{
			perror("Error in sending data-h3:\n");
			
		}

		/*Sending the read data over socket*/
		ret = sendto(sockfd,sendbuf,slen,0,to2,length);
		if (0 > ret)
		{
			perror("Error in sending data-h2:\n");
			
		}


		/*Sending the read data over socket*/
		ret = sendto(sockfd,sendbuf,slen,0,to1,length);
		if (0 > ret)
		{
			perror("Error in sending data-h1:\n");
			
		}

		printf("Data Sent\n");

	}
	close(ds->sd);	

}
//
void* receiver(void* lsock)

{
	int	sd, numbytes,bytessent, ret;
	struct	sockaddr_in serveraddress,cliaddr;
	socklen_t length;
	char clientname[MAXNAME],datareceived[BUFSIZE];

	sd = socket( AF_INET, SOCK_DGRAM, 0 );
	if(0 > sd ) 
	{
		perror( "socket" );
		
	}

	memset( &serveraddress, 0, sizeof(serveraddress) );
	memset( &cliaddr, 0, sizeof(cliaddr) );
	serveraddress.sin_family = AF_INET;
	serveraddress.sin_port = htons(MYPORT);//PORT NO
	serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);//IP ADDRESS
	ret=bind(sd,(struct sockaddr*)&serveraddress,sizeof(serveraddress));
	if(0 > ret)
	{
		perror("Bind Fails:");
		//exit(1);
	}
	for(;;)
	{
		printf("UDP Server:  Waiting for client data\n");
		length=sizeof(cliaddr);
		/*Received a datagram*/
		numbytes = recvfrom(sd,datareceived,BUFSIZE,0,
				(struct sockaddr*)&cliaddr, &length);
		if (0 > numbytes)
		{
			perror("Error while receiving:");
			//exit(1);
		}
		/*Printing client's address*/
		printf("Data Received from %s\n",
				inet_ntop(AF_INET,&cliaddr.sin_addr,
					clientname,sizeof(clientname)));
		
		/*Sending the Received datagram back*/
		datareceived[numbytes]='\0';
		printf("%s\n",datareceived);
		
	}
}

