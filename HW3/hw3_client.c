//2017112823 ¸ð±¤À±
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<fcntl.h>

#define BUF_SIZE 100
#define SEQ_START 1000

typedef struct{

	int seq;
	int ack;
	int buf_len;
	char buf[BUF_SIZE];
	
}Packet;
	
void error_handling(char* message);

int main(int argc, char* argv[]){

	int fd;
	Packet packet;
	
	int sock;
	
	struct sockaddr_in serv_addr;
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
		
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connet() error");
	
	char filename[BUF_SIZE];
	printf("Input file name: ");
	scanf("%s",filename);//server로부터 데이터를 읽어올 파일 이름 
	
	printf("[Client] request %s\n\n", filename);
	write(sock, filename, BUF_SIZE);//파일 이름 server로 전송
	
	int result_len;
	char result[BUF_SIZE];
	result_len = read(sock, result, sizeof(result));//전송한 파일 이름과 동일한 파일이 server가 있는 디렉토리에 있는지 여부 수신함
	if(result_len == -1)
		error_handling("read() error");
	
	if(strcmp(result, "File Not Found") == 0){//만약 파일이 없는 경우 종료
		printf("%s \n", result);
		exit(1);
	}
	
	fd = open(filename, O_CREAT | O_RDWR, 0644);//파일이 있을 경우 server에서 읽은 데이터를 저장할 파일 open (O_CREAT == 파일 없을 경우 생성)
	if(fd == -1)
		error_handling("open() error");
	
	int file_size = 0;
	while(1){
		result_len = read(sock, &packet.seq, sizeof(packet.seq));
		if(result_len == -1)
			error_handling("read() error");
			
		result_len = read(sock, packet.buf, BUF_SIZE);
		if(result_len == -1)
			error_handling("read() error");
			
		result_len = read(sock, &packet.buf_len, sizeof(packet.buf_len));
		if(result_len == -1)
			error_handling("read() error");
		
		write(fd, packet.buf, packet.buf_len);//생성한 파일에 읽어온 데이터 저자
		
		printf("[Client] Rx SEQ: %d, len: %d bytes\n", packet.seq, packet.buf_len);
		
		file_size += packet.buf_len;
		
		if(packet.buf_len < 100){
			printf("%s received (%d Bytes)\n", filename, file_size);
			break;
		}
		
		packet.ack = packet.seq + packet.buf_len + 1;
		printf("[Client] Tx ACK : %d\n\n", packet.ack);
		write(sock, &packet.ack, sizeof(packet.ack));
		
		
	}
	
	close(fd);
	close(sock);
}
void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
	
		
	
