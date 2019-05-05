#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<ctype.h>

void ComparingFiles(char *file1, char *file2);

int main(int argc, char **argv){

ComparingFiles(argv[1], argv[2]);

return 0;
}

void ComparingFiles(char *file1, char *file2){

int file1Size;
int file2Size;

int fd = open(file1, O_RDONLY);

if(fd < 0){
        printf("Did not have a file in the current directory.\n");
}

struct stat check;
if((stat(file1,&check)==0)){
         file1Size = check.st_size;
}
char *holder1 = (char*)malloc(sizeof(char)*file1Size+1);
read(fd, holder1, file1Size);

*(holder1 + file1Size) = '\0';
close(fd);


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

printf("%s\n", versionNumber[0]);
        for(i = 0; i < index; i++){
                printf("%s %s %s \n", versionNumber[i+1], fileName[i], HashContent[i]);
        }
        printf("\n");

int fd2 = open(file2, O_RDONLY);

if(fd2 < 0){
        printf("Did not have a second file to compare with the primary file in the directory.\n");
}

struct stat check2;
if(stat(file2, &check2) == 0){
        file2Size = check2.st_size;
}

char *holder2 = (char*)malloc(sizeof(char)*file2Size+1);
read(fd2, holder2, file2Size);

*(holder2 + file2Size) = '\0';
close(fd2);

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
	//versionNumber is client and versionNumber2 is server
	
	printf("%s\n", versionNumber2[0]);
        for(i = 0; i < index2; i++){
                printf("%s %s %s \n", versionNumber2[i+1], fileName2[i], HashContent2[i]);
        }


	if(atoi(versionNumber[0]) != atoi(versionNumber2[0])){
		printf("The versions do not match from the clients manifest version and the server manifest version.\nPlease update the local project first.\n");
	}
	
	/*else if(atoi(versionNumber[0]) == atoi(versionNumber2[0])){
		for(i = 0; i < index2; i++){
			for(j = 0; j < index; j++){
				if((strcmp(fileName[j], fileName2[i]) == 0) && atoi(versionNumber[j+1]) != atoi(versionNumber2[i+1])){
   	                                    printf("Allow the commit to happen.Client has newer version of the file.\n");
                                            break;
                                }
			}
	    	}
	}*/

	/*if(strcmp(fileName[3], fileName2[3]) == 0){
		printf("good.\n");
	}*/

}
