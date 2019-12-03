#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
void *do_chat(void *); //채팅 메세지를 보내는 함수
int pushClient(int); //새로운 클라이언트가 접속했을 때 클라이언트 정보 추가
int popClient(int); //클라이언트가 종료했을 때 클라이언트 정보 삭제
pthread_t thread;
pthread_mutex_t mutex;
#define MAX_CLIENT 10
#define CHATDATA 1024
#define INVALID_SOCK -1
#define PORT 9000
struct chat{
	int c_socket;
	char nickname[CHATDATA];
	char room[20];
};

struct chat list_c[MAX_CLIENT];
char    escape[ ] = "exit";
char    greeting[ ] = "Welcome to chatting room\n";
char    CODE200[ ] = "Sorry No More Connection\n";
int main(int argc, char *argv[ ])
{
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int    len;
    int    i, j, n;
    int    res;
	char nickname[CHATDATA];
    if(pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Can not create mutex\n");
        return -1;
    }
    s_socket = socket(PF_INET, SOCK_STREAM, 0);
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);
    if(bind(s_socket, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1) {
        printf("Can not Bind\n");
        return -1;
    }
    if(listen(s_socket, MAX_CLIENT) == -1) {
        printf("listen Fail\n");
        return -1;
    }
    for(i = 0; i < MAX_CLIENT; i++)
        list_c[i].c_socket = INVALID_SOCK;
    while(1) {
        len = sizeof(c_addr);
        c_socket = accept(s_socket, (struct sockaddr *) &c_addr, &len);
			

        res = pushClient(c_socket);
        if(res < 0) { //MAX_CLIENT만큼 이미 클라이언트가 접속해 있다면,
            write(c_socket, CODE200, strlen(CODE200));
            close(c_socket);
        } else {
            write(c_socket, greeting, strlen(greeting));
            pthread_create(&thread,NULL,do_chat,(void *)&c_socket);//pthread_create with do_chat function.
        }
    }
}
void *do_chat(void *arg)
{
    int c_socket = *((int *)arg);
    char chatData[CHATDATA];
	char nickname[20];
	char room[20]="all";
	char writeData[CHATDATA];
    int i, n;
	char *ptr;
	char g[20];
	for(i=0;i<MAX_CLIENT;i++){
		if(list_c[i].c_socket==c_socket){
			strcpy(nickname,list_c[i].nickname);
			}
	}
    while(1) {
        memset(chatData, 0, sizeof(chatData));
        if((n = read(c_socket, chatData, sizeof(chatData))) > 0) {
			sprintf(writeData,"[채팅방:%s],nickname:%s %s",room,nickname,chatData);
			
			if(strncasecmp(chatData,"/w",2)==0){
					strtok(chatData," ");
					strcpy(g,strtok(NULL," "));
					strcpy(chatData,strtok(NULL,"\0"));
					sprintf(writeData,"[%s] %s",nickname,chatData);
				for(i=0;i<MAX_CLIENT;i++){  
					if(strcasecmp(g,list_c[i].nickname)==0){
						write(list_c[i].c_socket,writeData,strlen(writeData));
						}
					}
			}
			else if(strncasecmp(chatData,"/join",5)==0){
					strtok(chatData," ");
					strcpy(room,strtok(NULL,"\0"));
					sprintf(writeData,"[%s] %s방\n",nickname,room);
					for(i=0;i<MAX_CLIENT;i++){  
							if(list_c[i].c_socket==c_socket){
								strcpy(list_c[i].room,room);
							}
					}
					for(i=0;i<MAX_CLIENT;i++){  
							if(strcasecmp(list_c[i].room,room)==0){
								write(list_c[i].c_socket,writeData,strlen(writeData));
							}
					}
				}
			else{
				for(i=0;i<MAX_CLIENT;i++){  
					if(list_c[i].c_socket!=INVALID_SOCK){         
						if(strcasecmp(list_c[i].room,room)==0){
							write(list_c[i].c_socket,writeData,strlen(writeData));
						}
				}
				}
			}
            	}
         }
     }
  

int pushClient(int c_socket) {
	
	int  i;
	char writeData[CHATDATA];
	char s[20];

	memset(s,0,sizeof(s));
	read(c_socket,s,sizeof(s));
	for(i=0;i<MAX_CLIENT;i++){
		if(list_c[i].c_socket==INVALID_SOCK){
			list_c[i].c_socket = c_socket;
			memset(list_c[i].nickname,0,sizeof(list_c[i].nickname));
			strcpy(list_c[i].nickname,s);
			memset(list_c[i].room,0,sizeof(list_c[i].room));
			strcpy(list_c[i].room,"all");
			sprintf(writeData,"[%s] 채팅방 접속\n",s);
			for(i=0;i<MAX_CLIENT;i++){
				if(list_c[i].c_socket==INVALID_SOCK){
				write(list_c[i].c_socket,writeData,strlen(writeData));
				}
			}
			return i;
		}
			
	}
		return -1;
}
	
int popClient(int c_socket)
{
	int i,j;
	char writeData[CHATDATA];
	for(i=0;i<MAX_CLIENT;i++){
		if(list_c[i].c_socket==c_socket){
			sprintf(writeData,"[%s] 채팅방 나가기\n",list_c[i].nickname);
			for(j=0;i<MAX_CLIENT;j++){
				if(list_c[j].c_socket!=INVALID_SOCK){
					write(list_c[j].c_socket,writeData,strlen(writeData));
				}
			}
			list_c[i].c_socket = INVALID_SOCK;
			strcpy(list_c[i].nickname,"\0");
	  	  	strcpy(list_c[i].room,"\0");	
		}
	}
	close(c_socket);

	
   
}
