#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *func_thread(void *argv);//쓰레드가 수행할 함수
//void * : java의 Object와 비슷한 개념. 모든타입으로 형변환 가능
int main(){
	int status;
	pthread_t thread_id;
	pthread_create(&thread_id,NULL,func_thread,NULL);
	
	printf("thread %x is created\n",thread_id);
	pthread_join(thread_id,(void **)&status); //자식쓰레드가 다실행될때까지 기다리고 다실행되면 메인쓰레드종료
	printf("main function END\n");
	return 0;
}
	

	void *func_thread(void *argv){
	int i=0;
	
	while(i<10){
	i++;
	printf("thread %dth executing....\n",i);
	}
}
	
