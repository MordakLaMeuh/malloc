
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char **argv)
{
	printf("BEGIN\n");
	if (argc != 2)
		return (-1);
	int fd;

	char *ptr = getenv("banane");
	printf("ptr is %p, str is %s\n", ptr, ptr);
	fd = open(argv[1], O_WRONLY | O_NONBLOCK);
	if (fd < 0)
	{
		printf("open Fail\n");
		perror("errno: ");
		return (-1);
	}
	printf("file opened !\n");
	int i = close(fd);
	printf("close return is %i\n", i);
	printf("END OF FILe\n");
	return (0);
}
