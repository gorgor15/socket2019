#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#define PORT 10001
#define BUFSIZE 100
char rcvBuffer[BUFSIZE];
char Buffer[BUFSIZE];
char Buffer2[BUFSIZE];
char cmpBuffer[2][BUFSIZE];
char *tok;
void do_service(int c_socket);
void sig_handler();
int cnt=0;
int main(){
    int c_socket, s_socket;
    struct sockaddr_in s_addr, c_addr;
    int len;
   	int pid;
    char buff[255];
	
	signal(SIGCHLD,sig_handler);//첫번째인자:시그널번호, 두번째인자:첫번째인자의 시그널이 발생했을때 실행되는함수명
    // 1. 서버 소켓 생성
    //서버 소켓 = 클라이언트의 접속 요청을 처리(허용)해 주기 위한 소켓
    s_socket = socket(PF_INET, SOCK_STREAM, 0); //TCP/IP 통신을 위한 서버 소켓 생성
   
    //2. 서버 소켓 주소 설정
    memset(&s_addr, 0, sizeof(s_addr)); //s_addr의 값을 모두 0으로  초기화
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP 주소 설정
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(PORT);

    //3. 서버 소켓바인딩
    if(bind(s_socket,(struct sockaddr *) &s_addr, sizeof(s_addr)) == -1){
        //바인딩 작업 실패 시, Cannot Bind 메시지 출력 후 프로그램 종료
        printf("Cannot Bind\n");
        return -1;
    }
   
    //4.listen() 함수 실행
    if(listen(s_socket, 5) == -1){
        printf("listen Fail\n");
        return -1;
    }

    //5. 클라이언트 요청 처리
    // 요청을 허용한 후, Hello World 메세지를 전송함
    while(1){ //무한 루프
        len = sizeof(c_addr);
        printf("클라이언트 접속을 기다리는 중....\n");
        c_socket = accept(s_socket, (struct sockaddr *)&c_addr, &len);
        printf("/client is connected\n");
		cnt++;
		printf("현재 접속중인 클라이언트수 : %d\n",cnt);
		
		if((pid=fork())>0){
		close(c_socket);
		continue;
		}
		else if(pid==0){
		close(s_socket);
	   	do_service(c_socket);
		cnt--;
		exit(0);
		}
		else{
			printf("fork() failed\n");
			exit(0);
		}
    	}

	return 0;
}

void do_service(int c_socket){
	 int n;
    int readSize;
    int count=0;
	 while(1){
		    memset(rcvBuffer,0,BUFSIZE);
		    memset(Buffer,0,BUFSIZE);
		    memset(Buffer2,0,BUFSIZE);
		    if((readSize=read(c_socket,rcvBuffer,sizeof(rcvBuffer)))<0){
		        return -1;
				    }
			       
				    printf("rcvBuffer : %s\n",rcvBuffer);
				    if(strncmp(rcvBuffer,"quit",4)==0){
				    break;
				    }
				    else if(strcasecmp(rcvBuffer,"안녕하세요") == 0){
				    strcpy(Buffer,"Hi NICE MEET YOU");
				    }
				    else if(strcasecmp(rcvBuffer,"너의이름은뭐니")== 0){
				    strcpy(Buffer,"My name is Kim Jong Heon");
				    }
				    else if(strcasecmp(rcvBuffer,"몇살이니")== 0){
				    strcpy(Buffer,"i'm 23");
				    }
				    else if(strncmp(rcvBuffer,"strlen ",strlen("strlen "))== 0){ //문자열비교
					    tok=strtok(rcvBuffer," ");
					    while(tok != NULL){
					    tok=strtok(NULL," ");
					    if(tok != NULL){
					    strcpy(Buffer2,tok);
					    sprintf(Buffer,"<%s> len : %d",Buffer2,strlen(Buffer2));
					    }
				    }   
				    }
		    else if(strncmp(rcvBuffer,"strcmp",6)== 0){
		            count=0;
		            tok = strtok(rcvBuffer," ");
	       
		            if(tok != NULL){
		                while(tok != NULL){
		                    tok = strtok(NULL," ");
		                    if(tok != NULL){
		                        strcpy(cmpBuffer[count],tok);
		                            count++;
		                    }
		                }
		            }
		        if(strcmp(cmpBuffer[0],cmpBuffer[1]) == 0){
		                strcpy(Buffer,"0");
		        }else{
		                strcpy(Buffer,"1");
		        }
		    }
	  n = strlen(Buffer);
    write(c_socket,Buffer,n);
          
    printf("send Data: %s\n",Buffer);
  }
        close(c_socket);
}
 
void sig_handler(){
	int pid;
	int status;
	pid=wait(&status);
	printf("pid[%d] terminated. status \ %d\n",pid,status);
	cnt--;
		printf("현재 접속중인 클라이언트수 : %d\n",cnt);
}
	
