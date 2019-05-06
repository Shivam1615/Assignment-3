/* server code history */

void history(char* direc)
{
//send history file
struct stat st;
char *act;
sprintf(act,"./.server_por/%s/.History",direc);
stat(act, &st);
int size = st.st_size;
//printf("%d\n",size);
write(sock,sprintf("%d",size),20);
int fd= open(act,O_RDONLY,0777);
char* buf;
int re=read(fd,buf,size);
write(sock,,size);
}

/* client code under history */
char* filesize;
read(sock,filesize,20);
char* historyFile;
read(sock,historyFile,atoi(filesize));
printf("%s\n",historyFile);
