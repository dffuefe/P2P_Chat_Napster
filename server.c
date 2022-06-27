#include <stdio.h>
#include <netinet/in.h>//소켓함수 사용
#include <stdlib.h>//인터넷 소켓주소 구조체 사용
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 1024

int main(int argc, char **argv){
	struct sockaddr_in servaddr, cliaddr;//인터넷 소켓주소 구조체
	int serv_sock, clia_sock;
	char message[BUFSIZE];
	int string_len;
	int cliaadd_size;
 
	if(argc < 2){
		printf("Usage: %s port \n", argv[0]);
		return -1;
	}
 
	if((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		//SOCK_STREAM은 TCP를 의미한다 SOCK_DGRAM은 UDP
		perror("Socket failed");
		return -1;
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[1]));//포트
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IP(운영체제 지정 IP)
	
	if(bind(serv_sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){//1번쨰 인자 소켓번호 , 2번째 소켓주소(구조체라 포인터로), 3번째 소켓주소 크기) 
		perror("bind failed");
		return -1;
	}
	
	if(listen(serv_sock, 3) == -1){//서버의 대기모드
		perror("listen failed");
		return -1;
	}
	
	cliaadd_size = sizeof(cliaddr);
	
	clia_sock = accept(serv_sock, (struct sockaddr*)&cliaddr, &cliaadd_size);
	if(clia_sock < 0){
		perror("accept failed");
		return -1;
	}
	
	//데이터 전송

	while((string_len = read(clia_sock, message, BUFSIZE)) != 0){
		write(clia_sock, message, string_len);
            write(1, message, string_len);
	}
	close(clia_sock); 

	return 0;
}