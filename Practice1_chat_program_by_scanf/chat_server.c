#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]){
	int serv_sock;
	int new_sock;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in new_addr;
	socklen_t new_addr_size;
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
		
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
		
	new_addr_size = sizeof(new_addr);
	new_sock = accept(serv_sock, (struct sockaddr*)&new_addr, &new_addr_size);
	if(new_sock == -1)
		error_handling("accept() error");
	
	int str_len;
	int str_len1;
	while(1){
		char message[100] = {};
		char message1[100] = {};
		
		printf("Server : ");
		scanf("%s",message);
		
		//write(new_sock, message, sizeof(message));
		str_len1 = write(new_sock, message, sizeof(message));
		if(str_len1 == -1)
			error_handling("write() error");
		
		
		if(strcmp(message, "quit") == 0)
			break;	
			
		str_len = read(new_sock, message1, sizeof(message1));
		if(str_len == -1)
			error_handling("read() error");
			
		printf("Client : %s\n", message1);
		if(strcmp(message1, "quit") == 0)
			break;
	}
	
	close(serv_sock);
	close(new_sock);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
	
