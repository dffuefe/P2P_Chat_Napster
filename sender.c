#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define DEFINED_KEY 0x15134911
#define DEFINED_KEY_SECOND 0x15134912

void main(int argc, char **argv){
	
	int msg_qid;
	struct{
		long mtype;
		char content[256];
	}msg;
	
	int msg_qid_2;
	struct{
		long mtype;
		char content[256];
	}msg_2;
	
	fprintf(stdout, "===============SENDER=================\n");
	
	if((msg_qid = msgget(DEFINED_KEY, IPC_CREAT | 0666)) < 0){//큐를 만들거나 오픈
		perror("msgget: "); 
		exit(-1);
	}
	printf("msg_qid = %d\n", msg_qid);
	
	if((msg_qid_2 = msgget(DEFINED_KEY_SECOND, IPC_CREAT | 0666)) < 0){
		perror("msgget: "); 
		exit(-1);
	}
	printf("msg_qid_2 = %d\n", msg_qid_2);
	
	msg.mtype = 1;
	//msg_2.mtype = 2;//1로해야되나 2로 해야되나 모르것네
	
	while(1){
		memset(msg.content, 0x0, 256);
		fgets(msg.content, 256, stdin);
		if(msgsnd(msg_qid, &msg, sizeof(msg.content), 0) < 0){//큐에 추가
			perror("msgsnd: ");
			exit(-1);
		}
		
		if(msg.content[0] == '.') break;
		
		//여기서부터 받는거
		memset(msg_2.content, 0x0, 256);
		if(msgrcv(msg_qid_2, &msg, 256, 0, 0) < 0){//큐에서 받기
			perror("msgrcv: ");
			exit(-1);
		}
		//if(msg_2.content[0] == '.') break;
		
		fputs(msg.content, stdout);
		
	}
	

	}


/*

*/