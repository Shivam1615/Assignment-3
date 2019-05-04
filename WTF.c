#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <openssl/sha.h>

void sendFile(char *dirName,char *name,int sock){
	chdir(dirName);
	int fSize;
	int pSize=strlen(dirName);
	int nSize=strlen(name);
	struct stat check;
	int fd =open(name, O_RDONLY);
	if(stat(name,&check)==0)
		fSize=check.st_size;

	char *file=(char*)malloc(sizeof(char)*fSize+1);
	read(fd,file,fSize);
	*(file+fSize)='\0';
	close(fd);
	chdir("..");
	write(sock,"a",2);
	write(sock,&pSize,sizeof(int));
	write(sock,dirName,pSize);	
	write(sock,&fSize,sizeof(int));
	write(sock, file, fSize);
	write(sock, &nSize,sizeof(int));
	write(sock,name,nSize);

}

char *hash(char *string){

        size_t length = strlen(string);
	char *output=(char*)malloc(sizeof(40));
	strcpy(output,"");
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1(string, length, hash);

        int i;
	char code[3];	
        for(i=0;i<SHA_DIGEST_LENGTH;i++){
               sprintf(code,"%02x",hash[i]);
		strcat(output,code);
        }
	return output;
}

void addOrRemoveFile(char *dirName,char *path,char command){

int fSize;
struct stat check;
int fd=open(path,O_RDWR);

if(stat(path,&check)==0)
	fSize=check.st_size;
char *file=(char*)malloc(sizeof(char)*fSize+1);
read(fd,file,fSize);
*(file+fSize)='\0';
close(fd);

chdir(dirName);
int mSize;
fd=open(".Manifest", O_RDONLY);
if(stat(".Manifest",&check)==0)
	mSize=check.st_size;
char *man=(char*)malloc(sizeof(char)*mSize+1);
read(fd,man,mSize);
*(man+mSize)='\0';
close(fd);

int arraySize=0;
int i;
for(i=0;i<mSize-1;i++){
	
	if(man[i]=='\n'){
		arraySize++;
	}

}
      
char *version[arraySize];
char *fileName[arraySize-1];
char *Hashcontent[arraySize-1];

char* token = strtok(man, " \n");
char *temp=(char*)malloc(strlen(token));
strcpy(temp,token);
version[0]=temp;
token=strtok(NULL, " \n");
int split=0;
int index=0;
while (token) {

	char *temp=(char*)malloc(strlen(token));
	strcpy(temp,token);
	
	if(split==0){
		version[index+1]=temp;
		token = strtok(NULL, " \n");
		split++;
	}else if(split==1){
		fileName[index]=temp;
		token = strtok(NULL, " \n");
		split++;
	}else if(split==2){
		Hashcontent[index]=temp;
		token = strtok(NULL, " \n");
		split=0;
		index++;
	}
	
}

if(command=='a'){
fd=open(".Manifest", O_CREAT | O_RDWR, 0400);
write(fd, version[0],strlen(version[0]));
write(fd, "\n",1);
int inManifest=0;
int k;
printf("%d\n",arraySize);
for(k=0;k<arraySize-1;k++){
	
	write(fd,version[k+1],strlen(version[k+1]));
	write(fd, " ",1);

	write(fd,fileName[k],strlen(fileName[k]));
	write(fd," ",1);
        if(strcmp(path,fileName[k])==0){
                inManifest=1;
                write(fd,hash(file),40);
        }else{
		write(fd,Hashcontent[k],strlen(Hashcontent[k]));
	}
	write(fd,"\n",1);

}

if(inManifest==0){
	write(fd,"1",1);
	write(fd, " ",1);
	write(fd,path,strlen(path));
	write(fd," ",1);
	write(fd,hash(file),40);
	write(fd,"\n",1);
}

write(fd,"\n",1);
close(fd);
printf("a");
}
if(command=='r'){
remove(".Manifest");
fd=open(".Manifest",O_CREAT| O_WRONLY, 0600);
write(fd, version[0],strlen(version[0]));
write(fd, "\n",1);

int k;
printf("%d\n",arraySize);
for(k=0;k<arraySize-1;k++){
        if(strcmp(fileName[k],path)==0){
                continue;
        }
        printf("%d ",k);
        write(fd,version[k+1],strlen(version[k+1]));
        write(fd, " ",1);
        write(fd,fileName[k],strlen(fileName[k]));
        write(fd," ",1);
        write(fd,Hashcontent[k],strlen(Hashcontent[k]));
        write(fd,"\n",1);

}
write(fd,"\n",1);

close(fd);
printf("r");
}

chdir("..");

}

void createProject(char *name,int sock){
	mkdir(name, ACCESSPERMS);
	int size=strlen(name);
	write(sock,"c",2);
	write(sock,&size,sizeof(int));
	write(sock,name,size);
	chdir(name);
	int fd=open(".Manifest",O_CREAT | O_WRONLY, 0600);
	if(fd==-1){
		printf("failed to make manifest");
	}
	write(fd,"0",1);
	write(fd,"\n",1);
	close(fd);
	chdir("..");
}

void destroyProject(char *name,int sock){
	char command[50];
	strcpy(command,"rm -r ");
	strcat(command,name);
	system(command);
	int size=strlen(name);
	write(sock,"d",2);
	write(sock,&size,sizeof(int));
	write(sock,name,size);

}

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
	if(strcmp(argv[1],"send")==0){
		sendFile(argv[2],argv[3],sock);
	}else if(strcmp(argv[1],"add")==0){
		addOrRemoveFile(argv[2],argv[3],'a');
	}else if(strcmp(argv[1],"remove")==0){
		addOrRemoveFile(argv[2],argv[3],'r');
	}else if(strcmp(argv[1],"create")==0){
		createProject(argv[2],sock);
	}else if(strcmp(argv[1],"destroy")==0){
		destroyProject(argv[2],sock);
	}
	
	/* close socket */
	close(sock);

	return 0;
}
