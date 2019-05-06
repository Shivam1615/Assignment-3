#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
typedef struct
{
	int sock;
	struct sockaddr address;
	int addr_len;
} connection_t;

void rollback(int new){

        struct stat check;
        int vSize;
        int fd=open(".Version", O_RDONLY);
        if(stat(".Version",&check)==0)
                vSize=check.st_size;
        char *vers=(char*)malloc(sizeof(char)*vSize+1);
        read(fd,vers,vSize);
        *(vers+vSize)='\0';
        close(fd);

        char *token=strtok(vers," \n");
        int old=atoi(token);

	char delete[15];

	char version[4];
	strcpy(version,"");

	int i;
	for(i=old;i>new;i--){
		strcpy(delete,"rm -r version");
		sprintf(version,"%d",i);
		strcat(delete,version);
		system(delete);
	
	}


	sprintf(version,"%d",new);

	
        remove(".Version");

        int fd2=open(".Version", O_CREAT | O_WRONLY, 0600);
        write(fd2,version,strlen(version));
        write(fd2,"\n",1);
        close(fd2);


        FILE *history=fopen(".History","a");
        fprintf(history,"Rollback:%s\n",version);
        fclose(history);

}
int version(){
	
	struct stat check;
	int vSize;
	int fd=open(".Version", O_RDONLY);
	if(stat(".Version",&check)==0)
		vSize=check.st_size;
	char *vers=(char*)malloc(sizeof(char)*vSize+1);
	read(fd,vers,vSize);
	*(vers+vSize)='\0';
	close(fd);

	char *token=strtok(vers," \n");
	int x=atoi(token);

	x++;

	char newVersion[4];
	strcpy(newVersion,"");
	sprintf(newVersion,"%d",x);

	remove(".Version");

	int fd2=open(".Version", O_CREAT | O_WRONLY, 0600);
	write(fd2,newVersion,strlen(newVersion));
	write(fd2,"\n",1);
	close(fd2);

	FILE *history=fopen(".History","a");
	fprintf(history,"push:%s\n",newVersion);
	fclose(history);
	return x;

}

void * process(void * ptr)
{
	char * buffer;
	int len;
	connection_t * conn;
	long addr = 0;

	if (!ptr) pthread_exit(0); 
	conn = (connection_t *)ptr;

	char *command;
	command = (char *)malloc(2*sizeof(char));

	read(conn->sock,command,2);

	if(strcmp(command,"p")==0){
	
	int pSize;
	read(conn->sock,&pSize,sizeof(int));
	char *project=(char*)malloc((pSize+1));
	read(conn->sock,project,pSize);
	int files;
	read(conn->sock,&files,sizeof(int));
	chdir(".server_repo");
	chdir(project);

	char folder[10];
	strcpy(folder,"");
	sprintf(folder,"version%d",version());

	mkdir(folder,0700);
	chdir(folder);

	int i;
	for(i=0;i<files+1;i++){
		int nSize;
		read(conn->sock,&nSize,sizeof(int));
		char *path=(char*)malloc((nSize+1));
		read(conn->sock,path,nSize);

		char *name=&path[pSize+1];

		int fSize;
		read(conn->sock,&fSize,sizeof(int));
		char *file=(char*)malloc(fSize+1);
		read(conn->sock,file,fSize);
		remove(name);
		int fd=open(name, O_CREAT | O_WRONLY, 0600);
		if(fd==-1){ printf("failed to create file");}
		write(fd,file,fSize);
		close(fd);

	}
	
	chdir("..");
	chdir("..");
	chdir("..");
	

	}else if(strcmp(command,"c")==0){
		int size;
		read(conn->sock,&size,sizeof(int));
		char *project=(char*)malloc((size+1)*sizeof(char));
		read(conn->sock,project,size);
		chdir(".server_repo");
		mkdir(project, ACCESSPERMS);
		chdir(project);

        	int fd=open(".Version",O_CREAT | O_WRONLY, 0600);
        	if(fd==-1){
                	printf("failed to make version");
        	}
        	write(fd,"0",1);
        	write(fd,"\n",1);
	        close(fd);

                int fd2=open(".History",O_CREAT | O_WRONLY, 0600);
                if(fd==-1){
                        printf("failed to make History");
                }
                write(fd2,"create: 0",9);
                write(fd2,"\n",1);
                close(fd2);

		chdir("..");
		chdir("..");
		free(project);
	}else if(strcmp(command,"d")==0){
		int size;
		read(conn->sock,&size,sizeof(int));
		char *project=(char*)malloc((size+1)*sizeof(char));
		read(conn->sock,project,size);
		chdir(".server_repo");
		
		char command[50];
		strcpy(command,"rm -r ");
		strcat(command,project);
		system(command);			

		chdir("..");
	}else if(strcmp(command,"u")==0){
		int length;
		read(conn->sock,&length,sizeof(int));
		char *path=(char*)malloc((length+1)*sizeof(char));
		read(conn->sock,path,length);
			
		chdir(".server_repo");
		struct stat check;
		int size;
		int fd=open(path, O_RDONLY,0600);
		if(stat(path,&check)==0)
			size=check.st_size;
		char *file=(char*)malloc(sizeof(char)*size+1);
		read(fd,file,size);
		*(file+size)='\0';
		close(fd);	

		write(conn->sock,&size,sizeof(int));
		write(conn->sock,file,size);	

		chdir("..");
		
	}else if(strcmp(command,"r")==0){

                int length;
                read(conn->sock,&length,sizeof(int));
                char *project=(char*)malloc((length+1)*sizeof(char));
                read(conn->sock,project,length);
		int back;
		read(conn->sock,&back,sizeof(int));
	
		chdir(".server_repo");
		chdir(project);

		rollback(back);
	

		chdir("..");
		chdir("..");	
		
	}
	
	/* close socket and clean up */
	close(conn->sock);
	free(conn);
	pthread_exit(0);
}

int main(int argc, char ** argv)
{
	int sock = -1;
	struct sockaddr_in address;
	int port;
	connection_t * connection;
	pthread_t thread;

	/* check for command line arguments */
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s port\n", argv[0]);
		return -1;
	}

	/* obtain port number */
	if (sscanf(argv[1], "%d", &port) <= 0)
	{
		fprintf(stderr, "%s: error: wrong parameter: port\n", argv[0]);
		return -2;
	}

	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0)
	{
		fprintf(stderr, "%s: error: cannot create socket\n", argv[0]);
		return -3;
	}

	/* bind socket to port */
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
	{
		fprintf(stderr, "%s: error: cannot bind socket to port %d\n", argv[0], port);
		return -4;
	}

	/* listen on port */
	if (listen(sock, 5) < 0)
	{
		fprintf(stderr, "%s: error: cannot listen on port\n", argv[0]);
		return -5;
	}

	printf("%s: ready and listening\n", argv[0]);
	
	while (1)
	{
		/* accept incoming connections */
		connection = (connection_t *)malloc(sizeof(connection_t));
		connection->sock = accept(sock, &connection->address, &connection->addr_len);
		if (connection->sock <= 0)
		{
			free(connection);
		}
		else
		{
			/* start a new thread but do not wait for it */
			pthread_create(&thread, 0, process, (void *)connection);
			pthread_detach(thread);
		}
	}
	
	return 0;
}
