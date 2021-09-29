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
	packet.seq = SEQ_START;
	
	int serv_sock;
	int clnt_sock;
	
	struct  sockaddr_in serv_addr;
	struct	sockaddr_in clnt_addr;
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
		
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
		
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock == -1)
		error_handling("accept error");
	
	printf("--------------------------------------\n");
	printf("File Transmission Server\n");
	printf("--------------------------------------\n");
	
	char filename[BUF_SIZE];
	int fname_len = read(clnt_sock, filename, BUF_SIZE);
	if(fname_len == -1)
		error_handling("read() error");
	
	char fail[] = "File Not Found";
	char suc[] = "File Found";
	fd = open(filename, O_RDWR);
	if(fd == -1){
		printf("%s File Not Found\n", filename);
		int len = write(clnt_sock, fail, sizeof(fail));
		if(len == -1)
			error_handling("write() error");
		exit(1);
	}
	write(clnt_sock, suc, sizeof(suc));
	
	printf("[Server] sending %s\n\n", filename);
	
	
	int file_size = 0;
	while(1){
		packet.buf_len = read(fd, packet.buf, BUF_SIZE);
		if(packet.buf_len <= 0)
			break;
		
		printf("[Server] Tx : SEQ : %d, %d byte data\n", packet.seq, packet.buf_len);
		write(clnt_sock, &packet.seq, sizeof(packet.seq));
		write(clnt_sock, packet.buf, sizeof(packet.buf));
		write(clnt_sock, &packet.buf_len, sizeof(packet.buf_len));
		
		file_size += packet.buf_len;
		
		if(packet.buf_len < 100){
			printf("%s sent (%d Bytes)\n", filename, file_size);
			break;
		}
			
		int ack_len = read(clnt_sock, &packet.ack, sizeof(packet.ack));
		if(ack_len == -1)
			error_handling("read() error");
		printf("[Server] Rx ACK: %d \n\n", packet.ack);
		packet.seq = packet.ack;
		
		
	}
	
	close(fd);
	close(serv_sock);
	close(clnt_sock);
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
	
	
	
	
	
