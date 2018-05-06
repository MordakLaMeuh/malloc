#include <stdlib.h>

int main()
{
	char *addr;

	addr = malloc(16);
	free(NULL);
	free((void *)addr + 5);
	return (0);
}
