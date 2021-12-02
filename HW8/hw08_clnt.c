//2017112823 ¸ð±¤À±
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include<semaphore.h>

#define ARSIZE 20

void * send_msg(void * arg);
void * recv_msg(void * arg);
void error_handling(char * msg);

static sem_t sem_one;
static sem_t sem_two;

typedef struct{
	int command;
	int seatno;
	int seats[ARSIZE];
	int result;
}BUS_INFO;
void printBus(BUS_INFO *bus);

BUS_INFO bus = {0};

int main(int argc, char *argv[])
{	
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	sem_init(&sem_one, 0, 0);
	sem_init(&sem_two, 0, 1);

	void * thread_return;
		
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	  
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");
	
	
	
			
		pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
		pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
		
	
	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);

	close(sock);  
	return 0;
}
	
void * send_msg(void * arg)   // send thread main
{
	int sock=*((int*)arg);
	int num;
	
	while(1){
	sem_wait(&sem_two);
	
	printf("1: inquiry, 2: reservation, 3: cancellation, 4: quit: ");
	scanf("%d",&num);
		
	bus.command = num;
	write(sock, &bus.command, sizeof(bus.command));
	
	if(bus.command == 1){}
	
	else if(bus.command == 2){
		printf("Input seat number: ");
		scanf("%d", &num);
		bus.seatno = num;
		write(sock, &bus.seatno, sizeof(bus.seatno));
	}
		
	else if(bus.command == 3){
		printf("Input seat number for cancellation: ");
		scanf("%d", &num);
		bus.seatno = num;
		write(sock, &bus.seatno, sizeof(bus.seatno));
	}
		
	else if(bus.command == 4){
		printf("Quit.\n");
		close(sock);
		exit(1);
	}

	sem_post(&sem_one);
	}
	return NULL;
}
	
void * recv_msg(void * arg)   // read thread main
{
	int sock=*((int*)arg);
	
	int str_len;
	
	while(1){
	sem_wait(&sem_one);
	
	str_len=read(sock, &bus, sizeof(bus));
	if(str_len==-1) 
		return (void*)-1;
	
	if(bus.command == 1){
		printBus(&bus);
	}
	
	else if(bus.command == 2){
		printBus(&bus);
	}
		
	else if(bus.command == 3){
		printBus(&bus);
	}
		
	else if(bus.command == 4){
	}
	
	sem_post(&sem_two);
	}
	return NULL;
}
	
void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

void printBus(BUS_INFO *bus){

	printf("---------------------------------------------------\n");

	for(int  i = 0; i < 10; i++)
		printf("%3d  ", i+1);

	printf("\n");

	for(int j = 0; j < 10; j++){
		printf("%3d  ", bus->seats[j]);
	}

	printf("\n---------------------------------------------------\n");

	for(int  k = 10; k < 20; k++)
		printf("%3d  ", k+1);

	printf("\n");

	for(int l = 10; l < 20; l++){
			printf("%3d  ", bus->seats[l]);
	}
	
	if(bus->result == 0)
		printf("\nOperation success.\n");
	else if(bus->result == -1)
		printf("\nWrong seat number.\n");
	else if(bus->result == -2)
		printf("\nReservation failed.(The seat was already reserved.)\n");
	else if(bus->result == -3)
		printf("\nCancellation failed.(The seat was not reserved.)\n");
	else
		printf("\nCancellation failed.(The seat was reserved by another person.)\n");
		
}
