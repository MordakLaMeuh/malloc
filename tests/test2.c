#include <stdio.h>

void *ft_malloc(size_t size);
void ft_free(void *ptr);
void	ft_debug_allocator(void);
void *ft_realloc(void *ptr, size_t size);

#define M (1024 * 1024)

#include <stdlib.h>
#include <string.h>
int main()
{
	int i;
	char *addr;

	i = 0;
	while (i < 1024)
	{
	addr = (char*)malloc(1024);
	addr[0] = 42;
	free(addr);
	i++;
	}
	return (0);
}
