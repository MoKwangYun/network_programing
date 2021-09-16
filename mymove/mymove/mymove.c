//2017112823 ¸ð±¤À± 

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
//#include<unistd.h>

#define BUF_SIZE 128

void error_handling(char* message);

int main(int argc, char* argv[]) {

	int fd;
	char buf[BUF_SIZE];
	fd = open(argv[1], O_RDONLY);

	int fd1;
	fd1 = open(argv[2], O_CREAT | O_RDWR, 0644);

	if (fd == -1 || fd1 == -1)
		error_handling("[Error] mymove Usage: ./mymove src_file dext_file");

	int size;
	int totalByte = 0;
	while ((size = read(fd, buf, BUF_SIZE)) > 0) {
		write(fd1, buf, size);
		totalByte += size;
	}
	printf("move from %s to %s (bytes: %d) finished\n", argv[1], argv[2], totalByte);

	

	close(fd);
	close(fd1);

	remove(argv[1]);

	return 0;

}

void error_handling(char* message) {

	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
