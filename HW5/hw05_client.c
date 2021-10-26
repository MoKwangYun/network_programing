//2017112823 ¸ð±¤À±
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/tcp.h>

#define BUF_SIZE 100

typedef struct{
	char name[BUF_SIZE];
	int proto_level;
	int opt_name;
	int optval;
	int result;
	int num;
}SO_PACKET;

void error_handling(char *message);

int main(int argc, char *argv[]){

	int sock;
	int str_len;
	socklen_t adr_sz;
	struct sockaddr_in serv_adr, from_adr;
	
	SO_PACKET pack;
	
	if(argc != 3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	
	if(sock == -1)
		error_handling("socket() error");
		
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(atoi(argv[2]));
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	
	while(1){
		
		int num;
		printf("--------------------------------\n");
		printf("1 : SO_SNDBUF\n");
		printf("2 : SO_RCVBUF\n");
		printf("3 : SO_REUSEADDR\n");
		printf("4 : SO_KEEPALIVE\n");
		printf("5 : SO_BROADCAST\n");
		printf("6 : IP_TOS\n");
		printf("7 : IP_TTL\n");
		printf("8 : TCP_NODELAY\n");
		printf("9 : TCP_MAXSEG\n");
		printf("10 : Quit\n");
		printf("--------------------------------\n");
		
		while(1){	
			printf("Input option number: ");
			scanf("%d",&pack.num);
		
			if(pack.num <= 0 || pack.num >10)
			{
				printf("Wrong number. type again!\n");
				continue;
			}
			
			else if(pack.num == 10){
				printf("Client quit.\n");
				close(sock);
				return 0;
			}
			else
				break;
		}
		
		printf("\nSending...\n");
		sendto(sock, &pack, sizeof(pack), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		
		printf("Send complete\n");
		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, &pack, sizeof(pack), 0, 
				(struct sockaddr*)&from_adr, &adr_sz);
			
		printf("recive complete\n");
		printf(">>> Server result option: %s, : value : %d, result: %d\n"
			, pack.name, pack.optval, pack.result);
	}
	
	close(sock);
	return 0;
}

void error_handling(char *message){
	
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
		
