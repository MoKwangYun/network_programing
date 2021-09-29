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
	scanf("%s",filename);
	
	printf("[Client] request %s\n\n", filename);
	write(sock, &filename, BUF_SIZE);
	
	int result_len;
	char result[BUF_SIZE];
	result_len = read(sock, result, sizeof(result));
	if(result_len == -1)
		error_handling("read() error");
	
	if(strcmp(result, "File Not Found") == 0){
		printf("%s \n", result);
		exit(1);
	}
	
	fd = open(filename, O_CREAT | O_RDWR, 0644);
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
		
		write(fd, packet.buf, packet.buf_len);
		
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
	
		
	
