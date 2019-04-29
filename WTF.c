#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char ** argv)
{
	int port;
	int sock = -1;
	struct sockaddr_in address;
	struct hostent * host;
	int len;
	
	/* checking commandline parameter */
	if (argc < 2)
	{
		fprintf(stderr,"Not enough input");
		return -1;
	}

	//configure client
	if(strcmp(argv[1],"configure")==0){
		if(argc!=4){
			fprintf(stderr,"incorrect input amount");
			exit(1);
		}
		int fd=open(".configure", O_CREAT | O_WRONLY, 0600);
		if(fd==-1){
			printf("Failed to configure");
			exit(1);
		}
		write(fd,argv[2],strlen(argv[2]));
		write(fd," ",1);
		write(fd,argv[3],strlen(argv[3]));
		close(fd);
		return 1;
	
	}

	FILE* conf = fopen(".configure","r");
	if(conf==NULL){
		printf("was not configured");
		exit(1);
	}

	char hostname[50];
	fscanf(conf,"%s",hostname);
	fscanf(conf,"%d",&port);




	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
		return -3;
	}

	/* connect to server */
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	host = gethostbyname(hostname);
	if (!host)
	{
		fprintf(stderr, "error: unknown host \n");
		return -4;
	}
	memcpy(&address.sin_addr, host->h_addr_list[0], host->h_length);
	if (connect(sock, (struct sockaddr *)&address, sizeof(address)))
	{
		fprintf(stderr, "error: cannot connect to host \n");
		return -5;
	}

	/* send text to server */
	len = strlen(argv[1]);
	write(sock,&len,sizeof(int));
	write(sock, argv[1], 100);

	/* close socket */
	close(sock);

	return 0;
}
