#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdlib.h>

#define BOARD_SIZE 3
#define INIT_VALUE 0
#define S_VALUE 1
#define C_VALUE 2

typedef struct{
	int board[BOARD_SIZE][BOARD_SIZE];
}GAMEBOARD;

void error_handling(char *message);

int available_space(GAMEBOARD *gboard);

void draw_board(GAMEBOARD *gboard);

int main(int argc, char *argv[]){

	int serv_sock;
	struct sockaddr_in serv_addr, clnt_adr;
	socklen_t clnt_adr_sz;
	GAMEBOARD gboard = {INIT_VALUE};
	int recv_size;
	
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("bind() error");
	
	printf("Tic-Tac-Toc Server\n");
	draw_board(&gboard);
	
	while(1){
		clnt_adr_sz = sizeof(clnt_adr);
		
		int full = available_space(&gboard);
		if(full == 0){
			printf("No available space. Exit this program\n");
			close(serv_sock);
			printf("Tic Tac Toc Server  Close\n");
			exit(1);
		}
			
		recv_size = recvfrom(serv_sock, &gboard, sizeof(gboard), 0, 
				(struct sockaddr*) &clnt_adr, &clnt_adr_sz);
		
		draw_board(&gboard);
		
		full = available_space(&gboard);
		if(full == 0){
			printf("No available space. Exit this program\n");
			close(serv_sock);
			printf("Tic Tac Toc client  Close\n");
			exit(1);
		}
		
		int x, y;
		while(1){
			x = rand()%3;
			y = rand()%3;
			printf("Server choose: [%d, %d]\n", x, y);
			
			
			//if(x >= BOARD_SIZE || x < 0){
			//	printf("Choose wrong number. Please try again\n");
			//	continue;
			//}
			//else if(y >= BOARD_SIZE || y < 0){
			//	printf("Choose wrong number. Please try again\n");
			//	continue;
			//}
			if(gboard.board[x][y] != INIT_VALUE){
				printf("Choose wrong number. Please try again\n");
				continue;
			}
			
			else{
				gboard.board[x][y] = S_VALUE;
				break;
			}
		}
		
		draw_board(&gboard);
		
		sendto(serv_sock, &gboard, sizeof(gboard), 0,
				(struct sockaddr*)&clnt_adr, clnt_adr_sz);
		
			  
	}
		
	
	
	close(serv_sock);
	return 0;
}

void error_handling(char *message){
	
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int available_space(GAMEBOARD *gboard){
	
	int i, j;
	
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
		
			if(gboard -> board[i][j] == INIT_VALUE)
				return 1;			
		}
	}
	
	return 0;
}

void draw_board(GAMEBOARD *gboard){
	
	char value = ' ';
	int i,j;
	printf("+------------+\n");
	
	for(i = 0; i < BOARD_SIZE; i++){
		for(j = 0; j < BOARD_SIZE; j++){
		
			if(gboard -> board[i][j] == INIT_VALUE)
				value = ' ';
			else if(gboard -> board[i][j] == S_VALUE)
				value = 'O';
			else if(gboard -> board[i][j] == C_VALUE)
				value = 'X';
			else
				value = ' ';
			
			printf("| %c ", value);
		}
		
		printf("|");
		printf("\n+------------+\n");
	}
}			
			
