#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

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

	int sock;
	struct sockaddr_in serv_adr, from_adr;
	socklen_t adr_sz;
	int recv_size;
	GAMEBOARD gboard = {INIT_VALUE};
	
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error_handling("socket() error");
		
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_adr.sin_port = htons(atoi(argv[2]));
	
	printf("Tic-Tac-Toc Client\n");
	draw_board(&gboard);
	
	while(1){
		
		int full = available_space(&gboard);
		if(full == 0){
			printf("No available space. Exit this program\n");
			close(sock);
			printf("Tic Tac Toc client  Close\n");
			exit(1);
		}
		
		int x, y;
		while(1){
			printf("Input row, column: ");
			scanf("%d %d", &x, &y);
			
			
			if(x >= BOARD_SIZE || x < 0){
				printf("Choose wrong number. Please try again\n");
				continue;
			}
			else if(y >= BOARD_SIZE || y < 0){
				printf("Choose wrong number. Please try again\n");
				continue;
			}
			
			else if(gboard.board[x][y] != INIT_VALUE){
				printf("Choose wrong number. Please try again\n");
				continue;
			}
			
			else{
				gboard.board[x][y] = C_VALUE;
				break;
			}
		}
		
		draw_board(&gboard);
		
		sendto(sock, &gboard, sizeof(gboard), 0, 
				(struct sockaddr*)&serv_adr, sizeof(serv_adr));
				
		full = available_space(&gboard);
		if(full == 0){
			printf("No available space. Exit this program\n");
			close(sock);
			printf("Tic Tac Toc client  Close\n");
			exit(1);
		}
		
		
		adr_sz = sizeof(from_adr);
		
		recv_size = recvfrom(sock, &gboard, sizeof(gboard), 0,
				(struct sockaddr*)&from_adr, &adr_sz);
			
		draw_board(&gboard);
		}
		
	close(sock);
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
