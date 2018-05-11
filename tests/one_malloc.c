#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void	*ft_malloc(size_t size);

int main(void)
{

	char *a = ft_malloc(16);
	(void)a;
	char *b = ft_malloc(40);
	(void)b;
	char *c = ft_malloc(30);
	(void)c;
	char *d = ft_malloc(1024);
	(void)d;
	char *e = ft_malloc(1024);
	(void)e;
	char *z = ft_malloc(1024 * 1024);
	(void)z;
	for (int i = 0; i < 1024; i++)
	{
		z = ft_malloc(64);
		memcpy(z, "banane", 7);
		printf("%i malloc done\n", i);
		if (memcmp(z, "banane", 7) != 0)
		{
			printf("READ ERROR\n");
			exit (1);
		}
	}
	ft_show_alloc_mem();
	return 0;
}
