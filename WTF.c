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

void sendFile(char *name,int sock){

	int nSize=strlen(name);
	write(sock,&nSize,sizeof(int));
	write(sock,name,nSize);

	int fSize;
	struct stat check;
	int fd=open(name, O_RDONLY);
	if(stat(name,&check)==0)
		fSize=check.st_size;
	char *file=(char*)malloc(fSize+1);
	read(fd,file,fSize);
	*(file+fSize)='\0';
	close(fd);

	write(sock,&fSize,sizeof(int));
	write(sock,file,fSize);
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
void commit(char * dirName){
	struct stat check;
	chdir(dirName);
	int mSize;
	int fd=open(".Manifest", O_RDONLY);
	if(stat(".Manifest",&check)==0)
        	mSize=check.st_size;
	char *man=(char*)malloc(sizeof(char)*mSize+1);
	read(fd,man,mSize);
	*(man+mSize)='\0';
	close(fd);
	chdir("..");

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
	char *temp=(char*)malloc(strlen(token)+1);
	strcpy(temp,token);
	version[0]=temp;
	token=strtok(NULL, " \n");
	int split=0;
	int index=0;
	while (token) {
        	char *temp=(char*)malloc(strlen(token)+1);
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
	chdir(dirName);
	remove(".Commit");
	fd=open(".Commit", O_CREAT | O_WRONLY,0600);
	
	int versionNumber=atoi(version[0]);
	versionNumber++;
	
	char newVersion[4];
	strcpy(newVersion,"");
	sprintf(newVersion,"%d",versionNumber);

	write(fd, newVersion,strlen(newVersion));
	write(fd, "\n",1);
	int k;

	for(k=0;k<arraySize-1;k++){
	
		versionNumber=atoi(version[k+1]);
		versionNumber++;	
		strcpy(newVersion,"");
		sprintf(newVersion,"%d",versionNumber);
	
		int fSize;
		char path[50];
		strcpy(path,fileName[k]);
		char name[20];
		strcpy(name,&path[strlen(dirName)+1]);
	
		int fd2=open(name,O_RDWR);

		if(stat(name,&check)==0)
        		fSize=check.st_size;
		char *file=(char*)malloc(sizeof(char)*fSize+1);
		read(fd2,file,fSize);
		*(file+fSize)='\0';
		close(fd2);

                char *hashCode=(char*)malloc(sizeof(char)*41);
                hashCode=hash(file);
 
/*		if(strcmp(hashCode,Hashcontent[k])==0){

		}else{
 */
        	write(fd,newVersion,strlen(newVersion));
        	write(fd," ",1);

        	write(fd,fileName[k],strlen(fileName[k]));
        	write(fd," ",1);

        	write(fd,hash(file),40);
        	write(fd,"\n",1);
		/*}*/
		
	}

	write(fd,"\n",1);
	close(fd);
	chdir("..");
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
char *temp=(char*)malloc(strlen(token)+1);
strcpy(temp,token);
version[0]=temp;
token=strtok(NULL, " \n");
int split=0;
int index=0;
while (token) {
	char *temp=(char*)malloc(strlen(token)+1);
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
fd=open(".Manifest", O_CREAT | O_RDWR, 0600);
write(fd, version[0],strlen(version[0]));
write(fd, "\n",1);
int inManifest=0;
int k;

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
	write(fd,"0",1);
	write(fd, " ",1);
	write(fd,path,strlen(path));
	write(fd," ",1);
	write(fd,hash(file),40);
	write(fd,"\n",1);
}

write(fd,"\n",1);
close(fd);
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

}

chdir("..");

}

void createProject(char *name,int sock){


	DIR* dir = opendir("mydir");
	if (dir)
	{
    		/* Directory exists. */
    		closedir(dir);
		printf("The project already exists");
	}
	else if (ENOENT == errno)
	{
    		/* Directory does not exist. */
	}

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

	/*configure client*/
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
	if(strcmp(argv[1],"push")==0){
		write(sock,"p",2);
		
		int pSize=strlen(argv[2]);
		write(sock,&pSize,sizeof(int));
		write(sock,argv[2],pSize);
		
		struct stat check;	
		chdir(argv[2]);
		int mSize;
		int fd=open(".Commit", O_RDONLY);

		if(fd==-1){
			printf("must commit first");
			return 1;
		}

		if(stat(".Commit",&check)==0)
        	mSize=check.st_size;
		char *man=(char*)malloc(sizeof(char)*mSize+1);
		read(fd,man,mSize);
		*(man+mSize)='\0';
		close(fd);

		

		remove(".Manifest");		
		int fd3=open(".Manifest",O_CREAT | O_WRONLY, 0600);
		write(fd3,man,mSize);
		close(fd3);


		chdir("..");
		int length=0;
		int i;
		for(i=0;i<mSize-1;i++){

        		if(man[i]=='\n'){
                	length++;
        		}

		}
		length--;
		write(sock,&length,sizeof(int));

		char manifest[20];
		strcpy(manifest,"");
		strcat(manifest,argv[2]);
		strcat(manifest,"/.Manifest");	

		sendFile(manifest,sock);
	
		char *token=strtok(man, " \n");
		token=strtok(NULL," \n");
		while(token){
			token=strtok(NULL," \n");
			
			sendFile(token,sock);
		
			token=strtok(NULL," \n");
			token=strtok(NULL," \n");
	
		}
	}else if(strcmp(argv[1],"add")==0){
		addOrRemoveFile(argv[2],argv[3],'a');
	}else if(strcmp(argv[1],"remove")==0){
		addOrRemoveFile(argv[2],argv[3],'r');
	}else if(strcmp(argv[1],"create")==0){
		createProject(argv[2],sock);
	}else if(strcmp(argv[1],"destroy")==0){
		destroyProject(argv[2],sock);
	}else if(strcmp(argv[1],"commit")==0){
		commit(argv[2]);
	}else if(strcmp(argv[1],"upgrade")==0){
		write(sock,"u",2);
		int length=strlen(argv[2]);
		
		write(sock,&length,sizeof(int));
		write(sock,argv[2],length);

		read(sock,&length,sizeof(int));
		char *file=(char*)malloc(sizeof(char)*length+1);
		read(sock,file,length);
		printf("%s\n",file);
	}else if(strcmp(argv[1],"rollback")==0){
		write(sock,"r",2);
		int length=strlen(argv[2]);
		int back=atoi(argv[3]);
	
		write(sock,&length,sizeof(int));
		write(sock,argv[2],length);

		write(sock,&back,sizeof(int));


	}
	
	/* close socket */
	close(sock);

	return 0;
}
