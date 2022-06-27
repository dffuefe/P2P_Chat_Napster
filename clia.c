#include <stdio.h>
#include <stdlib.h>//인터넷 소켓주소 구조체 사용
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 1024

int main(int argc, char **argv){
	struct sockaddr_in servaddr;//인터넷 소켓주소 구조체
	int sock;
	char message[BUFSIZE];
	int string_len;
	
	if(argc < 2){
		printf("Usage: %s port \n", argv[0]);
		return -1;
	}
 
	if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		//SOCK_STREAM은 TCP를 의미한다 SOCK_DGRAM은 UDP
		perror("Socket failed");
		return -1;
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));//포트
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);//IP(운영체제 지정 IP)
	
	if(connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
		perror("connection failed");
		return -1;
	}
	
	while(1){
		fputs("메시지를 입력하세요: ", stdout);
		fgets(message, BUFSIZE, stdin);
		
		if(!strcmp(message, "q\n")){
			break;
		}
		write(sock, message, strlen(message));	
		//여기까지 메시지 전송
		
		string_len = read(sock, message, BUFSIZE -1);
		message[string_len] = 0;
		printf("서버에서 전송: %s \n", message);
    }                 

	close(sock); 

	return 0;
}