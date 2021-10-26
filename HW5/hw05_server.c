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
	
	int serv_sock;
	int str_len;
	socklen_t clnt_adr_sz;
	struct sockaddr_in serv_adr, clnt_adr;
	SO_PACKET pack;
	socklen_t optlen;
	optlen = sizeof(pack.optval);
	
	int tcp_sock;
	tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
	
	if(argc != 2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	
	if(serv_sock == -1)
		error_handling("UDP socket creation error");
		
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(atoi(argv[1]));
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
		
	printf("Socket Option Server Start\n\n");
		
	while(1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		
		str_len = recvfrom(serv_sock, &pack, sizeof(pack), 0, 
					(struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		
		switch(pack.num){
		
			case 1:
			{
				pack.opt_name = SO_SNDBUF;
				pack.proto_level = SOL_SOCKET;
				strcpy(pack.name,"SO_SNDBUF");
				break;
			}
			case 2:
			{
				pack.opt_name = SO_RCVBUF;
				pack.proto_level = SOL_SOCKET;
				strcpy(pack.name, "SO_RCVBUF");
				break;
			}
			case 3:
			{
				pack.opt_name = SO_REUSEADDR;
				pack.proto_level = SOL_SOCKET;
				strcpy(pack.name,"SO_REUSEADDR");
				break;
			}
			case 4:
			{
				pack.opt_name = SO_KEEPALIVE;
				pack.proto_level = SOL_SOCKET;
				strcpy(pack.name, "SO_KEEPALIVE");
				break;
			}
			case 5:
			{
				pack.opt_name = SO_BROADCAST;
				pack.proto_level = SOL_SOCKET;
				strcpy(pack.name,"SO_BROADCAST");
				break;
			}
			case 6:
			{
				pack.opt_name = IP_TOS;
				pack.proto_level = IPPROTO_IP;
				strcpy(pack.name, "IP_TOS");
				break;
			}
			case 7:
			{
				pack.opt_name = IP_TTL;
				pack.proto_level = IPPROTO_IP;
				strcpy(pack.name, "IP_TTL");			
				break;
			}
			case 8:
			{
				pack.opt_name = TCP_NODELAY;
				pack.proto_level = IPPROTO_TCP;
				strcpy(pack.name, "TCP_NODELAY");
				break;
			}
			case 9:
			{
				pack.opt_name = TCP_MAXSEG;
				pack.proto_level = IPPROTO_TCP;
				strcpy(pack.name, "TCP_MAXSEG");
				break;
			}
						
		}
		
		printf(">>> Received Socket option: %s\n", pack.name);
		
		pack.result = getsockopt(tcp_sock, pack.proto_level, pack.opt_name, 
				(void*)&pack.optval, &optlen);		
		
		if(pack.result == -1)
			error_handling("getsockopt() error");
		
		printf("<<< Send option: %s, : %d, result: %d\n", pack.name, pack.optval, pack.result);
		
		sendto(serv_sock, &pack, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
		
	}
}

void error_handling(char *message){

	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);

}
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
