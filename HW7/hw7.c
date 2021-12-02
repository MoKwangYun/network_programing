//2017112823 ¸ð±¤À±
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>
#include<sys/types.h>

#define TTL 64
#define BUF_SIZE 128
void error_handling(char *message);

int main(int argc, char *argv[]){
	
	int send_sock, recv_sock;
	struct sockaddr_in s_adr, r_adr;
	int time_live = TTL;
	char buf[BUF_SIZE];
	int str_len, str_len1;
	struct ip_mreq join_adr;
	char name[BUF_SIZE];
	
	send_sock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&s_adr, 0, sizeof(s_adr));
	s_adr.sin_family = AF_INET;
	s_adr.sin_addr.s_addr = inet_addr(argv[1]);
	s_adr.sin_port = htons(atoi(argv[2]));
	
	setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
	
	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	
	int option = 1;
	setsockopt(recv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
	
	memset(&r_adr, 0, sizeof(r_adr));
	r_adr.sin_family = AF_INET;
	r_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	r_adr.sin_port = htons(atoi(argv[2]));
	
	if(bind(recv_sock, (struct sockaddr*)&r_adr, sizeof(r_adr)) == -1)
		error_handling("bind() error");
	
	join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
	join_adr.imr_interface.s_addr = htonl(INADDR_ANY);
	
	setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));
	
	
	
	pid_t pid = fork();
	
	
	while(1)
	{
		if(pid ==0)
		{
			str_len = recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);
			str_len1 = recvfrom(recv_sock, name, BUF_SIZE-1, 0, NULL, 0);
			
			if(str_len < 0 || str_len1 < 0)
				error_handling("recv error");
			buf[str_len] = 0;
			printf("Received Message: [%.*s]  %s",str_len1, name, buf);
		}
		else
		{
		
			fgets(buf, BUF_SIZE, stdin);
			sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&s_adr, sizeof(s_adr));
			sendto(send_sock, argv[3], strlen(argv[3]), 0, (struct sockaddr*)&s_adr, sizeof(s_adr));
		}
	}
	close(recv_sock);
	close(send_sock);
	printf("%s : end\n", argv[3]);	
	
	return 0;	
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
