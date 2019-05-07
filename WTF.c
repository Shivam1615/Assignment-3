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
#include <dirent.h>



void ComparingFiles(char *file1, char *file2){


char *holder1 = (char*)malloc(sizeof(char)*strlen(file1)+1);
strcpy(holder1,file1);


int newLine = 0;

int k = 0;
int i;

for(i = 0; i < strlen(holder1)-1; i++){
        if(holder1[i] == '\n'){
                newLine++;
        }
}

char *versionNumber[newLine+1];
char *fileName[newLine];
char *HashContent[newLine];

char* token = strtok(holder1, " \n");
char *temp=(char*)malloc(strlen(token)+1);
strcpy(temp,token);
versionNumber[0]=temp;
token=strtok(NULL, " \n");
int split=0;
int index=0;
while (token) {
        char *temp=(char*)malloc(strlen(token)+1);
        strcpy(temp,token);

        if(split==0){
                versionNumber[index+1]=temp;
                token = strtok(NULL, " \n");
                split++;
        }else if(split==1){
                fileName[index]=temp;
                token = strtok(NULL, " \n");
                split++;
        }else if(split==2){
                HashContent[index]=temp;
                token = strtok(NULL, " \n");
                split=0;
                index++;
        }
}

/*printf("%s\n", versionNumber[0]);
        for(i = 0; i < index; i++){
                printf("%s %s %s \n", versionNumber[i+1], fileName[i], HashContent[i]);
        }
	printf("\n");
*/
	
char *holder2 = (char*)malloc(sizeof(char)*strlen(file2)+1);
strcpy(holder2,file2);

int newLine2 = 0;

for(i = 0; i < strlen(holder2)-1; i++){
        if(holder2[i] == '\n'){
                newLine2++;
        }
}

char *versionNumber2[newLine2+1];
char *fileName2[newLine2];
char *HashContent2[newLine2];

char* token2 = strtok(holder2, " \n");
char *temp2=(char*)malloc(strlen(token2)+1);
strcpy(temp2,token2);
versionNumber2[0]=temp2;
token2=strtok(NULL, " \n");
int split2=0;
int index2=0;
while (token2) {
        char *temp2=(char*)malloc(strlen(token2)+1);
        strcpy(temp2,token2);
        if(split2==0){
                versionNumber2[index2+1]=temp2;
                token2 = strtok(NULL, " \n");
                split2++;
        }else if(split2==1){
                fileName2[index2]=temp2;
                token2 = strtok(NULL, " \n");
                split2++;
        }else if(split2==2){
                HashContent2[index2]=temp2;
                token2 = strtok(NULL, " \n");
                split2=0;
                index2++;
        }
}
        int j;
 
	char pointer[2] = " ";
	char pointer2[2] = "\n";
	char pointer4[5] = "U";
	char pointer3[5] = "M";	
	char pointer1[5] = "A";
	char pointer0[5] = "D";
	
	int count = 0; //for up to date
	
	if(atoi(versionNumber2[0]) > atoi(versionNumber[0])){

			char files[20];
        		strcpy(files, file1);
        		strcat(files, ".Update");
			int fd3 = open(files, O_CREAT | O_WRONLY, 0600);

			for(i = 0; i < index2; i++){
				int found=0;
				for(j = 0; j < index; j++){	
					if(((strcmp(fileName[j], fileName2[i]) == 0) && atoi(versionNumber2[i+1]) > atoi(versionNumber[j+1]) && atoi(versionNumber[0]) != atoi(versionNumber2[0]))){
						//printf("M %s %s %s \n", versionNumber2[i+1], fileName2[i],  HashContent2[i]);
						write(fd3, pointer3, strlen(pointer3));
						write(fd3, pointer, strlen(pointer));
						write(fd3, versionNumber2[j+1], strlen(versionNumber2[j+1]));
						write(fd3, pointer, strlen(pointer));
						write(fd3, fileName2[j], strlen(fileName2[j]));
						write(fd3, pointer, strlen(pointer));
						write(fd3, HashContent2[j], strlen(HashContent[j]));
						write(fd3, pointer2, strlen(pointer2));
						found=1; 
						break;
					}
				}
				if(found==0){
					//printf("A %s %s %s \n",versionNumber2[i+1],fileName2[i], HashContent2[i]);
					write(fd3, pointer1, strlen(pointer1));
                                        write(fd3, pointer, strlen(pointer));
                                        write(fd3, versionNumber2[i+1], strlen(versionNumber2[i+1]));
                                        write(fd3, pointer, strlen(pointer));
                                        write(fd3, fileName2[i], strlen(fileName2[i]));
                                        write(fd3, pointer, strlen(pointer));
                                        write(fd3, HashContent2[i], strlen(HashContent2[i]));
                                        write(fd3, pointer2, strlen(pointer2));        
				}
			}
			for(i = 0; i < index; i++){
				int found=0;
				for(j = 0; j < index2; j++){
					if(strcmp(fileName[i], fileName2[j]) == 0){
					found=1;
					break;
					}
				}
				if(found==0){
					if(strcmp(fileName[i], fileName2[i]) != 0 && atoi(versionNumber[i]) != atoi(versionNumber2[i])){
						//printf("D %s %s %s \n", versionNumber[i], fileName[i], HashContent[i]);
						write(fd3, pointer0, strlen(pointer0));
                                        	write(fd3, pointer, strlen(pointer));
                                        	write(fd3, versionNumber[i], strlen(versionNumber[i]));
                                        	write(fd3, pointer, strlen(pointer));
                                        	write(fd3, fileName[i], strlen(fileName[i]));
                                        	write(fd3, pointer, strlen(pointer));
                                        	write(fd3, HashContent[i], strlen(HashContent[i]));
                                        	write(fd3, pointer2, strlen(pointer2));
					}else{
						//printf("U %s %s %s \n", versionNumber[i], fileName[i], HashContent[i]);
						/*write(fd3, pointer4, strlen(pointer4));
                                                write(fd3, pointer, strlen(pointer));
                                                write(fd3, versionNumber[i], strlen(versionNumber[i]));
                                                write(fd3, pointer, strlen(pointer));
                                                write(fd3, fileName[i], strlen(fileName[i]));
                                                write(fd3, pointer, strlen(pointer));
                                                write(fd3, HashContent[i], strlen(HashContent[i]));
                                                write(fd3, pointer2, strlen(pointer2));
						*/
					}
				}
			} 
		}
		else if(index == index2 && strcmp(versionNumber[0], versionNumber2[0]) == 0){
			for(i = 0; i < index; i++){
				for(j = 0; j < index2; j++){
					if(atoi(versionNumber[i]) == atoi(versionNumber2[j]) && strcmp(fileName[i], fileName2[j]) == 0 && strcmp(HashContent[i], HashContent2[j]) == 0){
						count++;
						break;
					}
				}
			}
			if(count == index || count == index2){
				printf("Up to date.\n");
			}
		}
}





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
 
		if(strcmp(hashCode,Hashcontent[k])==0){

		write(fd,version[k],strlen(version[k]));
		write(fd," ",1);

		}else{
 
        	write(fd,newVersion,strlen(newVersion));
        	write(fd," ",1);
		}
        	write(fd,fileName[k],strlen(fileName[k]));
        	write(fd," ",1);

        	write(fd,hashCode,40);
        	write(fd,"\n",1);
		
		
	}

	write(fd,"\n",1);
	close(fd);
	chdir("..");
}

void addOrRemoveFile(char *dirName,char *path,char command){



        DIR* dir =opendir(dirName);
        if(!dir){
                closedir(dir);
                printf("The project doesn't exist\n");
                exit(1);

        }

int fSize;
struct stat check;
int fd=open(path,O_RDWR);
if(fd==-1){

	printf("File doesn't exist");
	exit(1);

}

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
	write(fd,"1",1);
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


	DIR* dir = opendir(name);
	if (dir)
	{
    		/* Directory exists. */
    		closedir(dir);
		printf("The project already exists\n");
		exit(1);
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

	DIR* dir =opendir(name);
	if(!dir){
		closedir(dir);
		printf("The project doesn't exist\n");
		exit(1);

	}


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

	/* These handle the commands from the server*/
	if(strcmp(argv[1],"push")==0){
		
		write(sock,"p",2);
		
        	DIR* dir =opendir(argv[2]);
        	if(!dir){
                	closedir(dir);
                	printf("The project doesn't exist\n");
                	exit(1);
        	}

		int pSize=strlen(argv[2]);
		write(sock,&pSize,sizeof(int));
		write(sock,argv[2],pSize);
		
		struct stat check;	
		chdir(argv[2]);
		int mSize;
		int fd=open(".Commit", O_RDONLY);

		if(fd==-1){
			printf("must commit first");
			exit(1);
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

		remove(".Commit");		

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
	}else if(strcmp(argv[1],"history")==0){
		

        DIR* dir =opendir(argv[2]);
        if(!dir){
                closedir(dir);
                printf("The project doesn't exist\n");
                exit(1);

        }


		write(sock,"h",2);
		int length=strlen(argv[2]);
		
		write(sock,&length,sizeof(int));
		write(sock,argv[2],length);

		int size;
		read(sock,&size,sizeof(int));	
		char *history=(char*)malloc(size+1);
		*(history+size)='\0';
		read(sock,history,size);
		printf("%s\n",history);
	}else if(strcmp(argv[1],"rollback")==0){


        DIR* dir =opendir(argv[2]);
        if(!dir){
                closedir(dir);
                printf("The project doesn't exist\n");
                exit(1);

        }

		write(sock,"r",2);
		int length=strlen(argv[2]);
		int back=atoi(argv[3]);
	
		write(sock,&length,sizeof(int));
		write(sock,argv[2],length);

		write(sock,&back,sizeof(int));

	}else if(strcmp(argv[1],"currentversion")==0){

        	DIR* dir =opendir(argv[2]);
        	if(!dir){
                	closedir(dir);
                	printf("The project doesn't exist\n");
                	exit(1);

        	}

                write(sock,"v",2);
                int length=strlen(argv[2]);

                write(sock,&length,sizeof(int));
                write(sock,argv[2],length);

                int size;
                read(sock,&size,sizeof(int));
                char *version=(char*)malloc(size+1);
                *(version+size)='\0';
                read(sock,version,size);
                printf("%s\n",version);

	}else if(strcmp(argv[1],"upgrade")==0){

        DIR* dir =opendir(argv[2]);
        if(!dir){
                closedir(dir);
                printf("The project doesn't exist\n");
                exit(1);

        }
                write(sock,"u",2);
                int pSize=strlen(argv[2]);

                write(sock,&pSize,sizeof(int));
                write(sock,argv[2],pSize);

               	int length;
		read(sock,&length,sizeof(int));

        int i;
        for(i=0;i<length;i++){
                int nSize;
                read(sock,&nSize,sizeof(int));
                char *path=(char*)malloc((nSize+1));
                read(sock,path,nSize);
		*(path+nSize)='\0';

                int fSize;
                read(sock,&fSize,sizeof(int));
                char *file=(char*)malloc(fSize+1);
                read(sock,file,fSize);
		*(file+fSize)='\0';
		
		char dir[50];
		strcpy(dir,argv[2]);
		strcat(dir,"/");   
		strcat(dir,path);
		
                remove(dir);
                int fd=open(dir, O_CREAT | O_WRONLY, 0600);
                if(fd==-1){ printf("failed to create file");}
                write(fd,file,fSize);
                close(fd);
       } 

        
                
        }else if(strcmp(argv[1],"update")==0){

                DIR* dir =opendir(argv[2]);
                if(!dir){
                        closedir(dir);
                        printf("The project doesn't exist\n");
                        exit(1);

                }

                write(sock,"v",2);
                int length=strlen(argv[2]);

                write(sock,&length,sizeof(int));
                write(sock,argv[2],length);

                int size;
                read(sock,&size,sizeof(int));
                char *server=(char*)malloc(size+1);
                *(server+size)='\0';
                read(sock,server,size);

		chdir(argv[2]);

	
        	int mSize;
        	int fd=open(".Manifest", O_RDONLY);
        	if(stat(".Manifest",&check)==0)
                	mSize=check.st_size;
        	char *man=(char*)malloc(sizeof(char)*mSize+1);
        	read(fd,man,mSize);
        	*(man+mSize)='\0';
        	close(fd);

		ComparingFiles(man,server);


		chdir("..");

	}else if(strcmp(argv[1],"checkout")==0){	


	        DIR* dir = opendir(name);
	        if (dir)
	        {
                /* Directory exists. */
        	        closedir(dir);
                	printf("The project already exists\n");
               		exit(1);
        	}
		
	        mkdir(argv[2], ACCESSPERMS);
	        int size=strlen(argv[2]);
	        chdir(argv[2]);
	        int fd=open(".Manifest",O_CREAT | O_WRONLY, 0600);
	        if(fd==-1){
         	       printf("failed to make manifest");
       	 	}
        	write(fd,"0",1);
        	write(fd,"\n",1);
        	close(fd);
        	chdir("..");

                write(sock,"u",2);
                int pSize=strlen(argv[2]);

                write(sock,&pSize,sizeof(int));
                write(sock,argv[2],pSize);

                int length;
                read(sock,&length,sizeof(int));

        int i;
        for(i=0;i<length;i++){
                int nSize;
                read(sock,&nSize,sizeof(int));
                char *path=(char*)malloc((nSize+1));
                read(sock,path,nSize);
                *(path+nSize)='\0';

                int fSize;
                read(sock,&fSize,sizeof(int));
                char *file=(char*)malloc(fSize+1);
                read(sock,file,fSize);
                *(file+fSize)='\0';

                char dir[50];
                strcpy(dir,argv[2]);
                strcat(dir,"/");
                strcat(dir,path);

                remove(dir);
                int fd=open(dir, O_CREAT | O_WRONLY, 0600);
                if(fd==-1){ printf("failed to create file");}
                write(fd,file,fSize);
                close(fd);
       }
		
	}
	
	/* close socket */
	close(sock);

	return 0;
}
