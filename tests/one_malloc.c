#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void	*ft_malloc(size_t size);
void	ft_free(void *ptr);

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

	ft_free(a);
	ft_free(c);
	ft_free(b);
	ft_free(d);

	ft_free(z);
	a = ft_malloc(400);

	b = ft_malloc(400);
	c = ft_malloc(400);
	ft_free(b);
	ft_free(a);
	ft_free(c);

	void *tab[32000];
	int i = 0;

	i = 0;
	for (; i < 24000; i++)
	{
		tab[i] = ft_malloc(1024);
		memcpy(tab[i], "banane", 7);
		if (memcmp(tab[i], "banane", 7) != 0)
		{
			printf("READ ERROR\n");
			exit (1);
		}
		printf("%i malloc done\n", i);
	}

	i = 0;
	for (; i < 24000; i++)
	{
		ft_free(tab[i]);
		printf("%i free done\n", i);
	}


	for (; i < 800; i++)
	{
		tab[i] = ft_malloc(89);
		memcpy(tab[i], "banane", 7);
		if (memcmp(tab[i], "banane", 7) != 0)
		{
			printf("READ ERROR\n");
			exit (1);
		}
		printf("%i malloc done\n", i);
	}

	for (; i < 1500; i++)
	{
		tab[i] = ft_malloc(45);
		memcpy(tab[i], "banane", 7);
		if (memcmp(tab[i], "banane", 7) != 0)
		{
			printf("READ ERROR\n");
			exit (1);
		}
		printf("%i malloc done\n", i);
	}

	for (; i < 2400; i++)
	{
		tab[i] = ft_malloc(1024);
		memcpy(tab[i], "banane", 7);
		if (memcmp(tab[i], "banane", 7) != 0)
		{
			printf("READ ERROR\n");
			exit (1);
		}
		printf("%i malloc done\n", i);
	}

	i = 0;
	for (; i < 2400; i++)
	{
		ft_free(tab[i]);
		printf("%i free done\n", i);
	}

	z = ft_malloc(124542);
	ft_show_alloc_mem();
	return (0);
}
