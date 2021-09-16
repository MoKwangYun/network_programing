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
	/*���� �ּ�ó�� �κ��̶� ������ ���
	read �Լ��� ���ϵ�ũ���� fd���� BUF_SIZE��ŭ ������ �а� �б� ������ ���� ����Ʈ �� ����
	�̸� totalByte�� ������ ���ư��鼭 ���� read�� �ݺ��Ͽ� fd��  BUF_SIZE��ŭ�� �����Ͱ� �������� �ʴٸ�
	�����ִ� ��ŭ�� �а� ���� ��ŭ�� ����Ʈ�� ����
	�� ���� read������ �� �̻� ���� �� �ִ� �����Ͱ� ����  ������ -1 �����ϰ� while���� ����������
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
