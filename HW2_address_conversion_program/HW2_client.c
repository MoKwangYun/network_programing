#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]){
	
	
	
	int sock; //sock == serv_sock
		
	struct sockaddr_in serv_addr;
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
		
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr= inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");
	
	int str_len;
	
	
	while(1){
	char  str[30] ={};
	char result[100] = {};
	printf("Input dotted-decimal address: ");
	scanf("%s",str);
	
	
	if(write(sock, str,sizeof(str)) == -1)
		error_handling("write() error");
		
	if(strcmp(str, "quit") == 0)
		break;
		
	str_len = read(sock, result, sizeof(result));
	if(str_len == -1)
		error_handling("read() error");
	
	printf("%s\n\n", result);
	
	}
	
	printf("quit!\n");
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
