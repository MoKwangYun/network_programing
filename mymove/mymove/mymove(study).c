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

	/*int size;
	size = read(fd, buf, sizeof(buf));
	if (size == -1)
		error_handling("read() error");

	if (write(fd1, buf, size) == -1)
		error_handling("write() error");


	printf("move from %s to %s (bytes: %d) finished\n", argv[1], argv[2], size);*/

	int readSize;
	int totalByte = 0;
	while ((readSize = read(fd, buf, BUF_SIZE)) > 0) {
		write(fd1, buf, readSize);
		totalByte += readSize;
	}
	printf("move from %s to %s (bytes: %d) finished\n", argv[1], argv[2], totalByte);
	/*위의 주석처리 부분이랑 동일한 결과
	read 함수는 파일디스크립터 fd에서 BUF_SIZE만큼 데이터 읽고 읽기 성공시 읽은 바이트 수 리턴
	이를 totalByte에 누적해 나아가면서 만약 read를 반복하여 fd에  BUF_SIZE만큼의 데이터가 남아있지 않다면
	남아있는 만큼만 읽고 읽은 만큼의 바이트수 리턴
	그 다음 read에서는 더 이상 읽을 수 있는 데이터가 없기  때문에 -1 리턴하고 while문을 빠져나간다
	*/

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
