//2017112823 ¸ð±¤À±
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<fcntl.h>
#include<sys/stat.h>

#define BUF_SIZE 2048
void error_handling(char *message);

int main(int argc, char *argv[])
{	
	int fd1;
	fd1 = open("rfc1180.txt", O_RDONLY);
	
	if(fd1 == -1)
		error_handling("open() error");
	
	
	int fd2;
	char buf[BUF_SIZE];
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;
	
	struct timeval timeout;
	fd_set reads, cpy_reads;
	int fd_max, fd_num;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	fd2=socket(PF_INET, SOCK_STREAM, 0);   
	if(fd2==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	FD_ZERO(&reads);
	FD_SET(fd1, &reads);
	
	fd_max = fd1; 
	
   	
	if(connect(fd2, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");
	
	
	
	while((str_len = read(fd1, buf, BUF_SIZE)) > 0)
        {
        
		cpy_reads = reads;
       	timeout.tv_sec = 3;
       	timeout.tv_usec = 3000;
       	if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout))== -1)
       		break;
     	 	if(fd_num == 0)
     	 		continue;
        	
		for(int i=0; i<fd_max+1; i++)
        	{	
            		if(FD_ISSET(i, &cpy_reads))
            		{	
               	 	
               		if(i == fd1)
                		{	
                			
                			FD_SET(fd2, &cpy_reads);
     						if(fd2 > fd_max)
   							fd_max = fd2;		
                			write(fd2, buf, strlen(buf));
                			sleep(1);
                			
	
               		 }
               		else if(i == fd2)  
                		{

					int str_len1=read(fd2, message, BUF_SIZE-1);
					if(str_len1 == 0)
					{
						FD_CLR(i,&reads);
						close(i);			
					}
					else
					{
						message[str_len1]=0;
						printf("%.*s",str_len1,message);
					}
                    
                		}
                		
            		}
            	
        	}
      
        }
        if(str_len == -1)
                error_handling("read() error");
        else if(str_len == 0)
        {	printf("\n\nstr_len\n\n");
                				
                FD_CLR(fd1, &reads);     
        	close(fd1);
        }
	
	
	close(fd2);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
