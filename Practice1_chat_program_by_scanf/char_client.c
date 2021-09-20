#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]){
	
	int clnt_sock;
	
	struct sockaddr_in serv_addr;
	
	clnt_sock = socket(PF_INET,SOCK_STREAM, 0);
	if(clnt_sock == -1)
		error_handling("socket() error");
		
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == - 1)
		error_handling("connet() error");
	
	int str_len;
	int str_len1;
	while(1){
		char message[100] = {};
		char message1[100] = {};
		
		str_len = read(clnt_sock, message, sizeof(message));
		if(str_len == -1)
			error_handling("read() error");
			
		printf("Server : %s\n", message);
		if(strcmp(message, "quit") == 0)
			break;
		
		printf("Client : ");
		scanf("%s",message1);
		
		//write(clnt_sock, message1, sizeof(message1));
		str_len1 = write(clnt_sock, message1, sizeof(message1));
		if(str_len1 == -1)
			error_handling("write() error");
		
		if(strcmp(message1, "quit") == 0)
			break;	
	}
	
	close(clnt_sock);
	
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
