#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]){

	printf("------------------------------\n");
	printf("  Address Conversion Server\n");
	printf("------------------------------\n");
	
	int serv_sock;
	int clnt_sock;
	
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	
	struct sockaddr_in addr_inet; //struct for store str that send from client
	
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
		
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	
	if(bind(serv_sock,(struct sockaddr*) &serv_addr, sizeof(serv_addr) )== -1)
		error_handling("bind() error");
		
	if(listen(serv_sock, 5) == -1)
		error_handling("listen() error");
		
	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_addr_size);
	if(clnt_sock == -1)
		error_handling("accept() error");
	
		
	char str[30];
	int str_len;
	
	char result2[] = "Address conversion fail : Format error";
	char result1[] = "Address conversion success";
	char *str1;
	
	while(1){
		str_len= read(clnt_sock, str, sizeof(str));
		if(str_len == -1)
			error_handling("read() error");
		
	
	
		if(strcmp(str,"quit") == 0){
			printf("quit received and exit program!\n");
			break;
			}
	
		printf("\nReceived Dotted-Decimal Address : %s\n",str);
	
		if(!inet_aton(str, &addr_inet.sin_addr)){
			printf("Address conversion fail : Format error\n\n");
			write(clnt_sock, result2, sizeof(result2));
			
			
		}
	
		else{
			printf("inet_aton : %s -> %#x \n", str, addr_inet.sin_addr.s_addr);	
		
			str1 = inet_ntoa(addr_inet.sin_addr);
			printf("inet_nota : %#x -> %s\n", addr_inet.sin_addr.s_addr, str1);
			write(clnt_sock, result1, sizeof(result1));
			printf("%s\n\n", result1);
		}
		
		
	
	
	}
	
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
	
