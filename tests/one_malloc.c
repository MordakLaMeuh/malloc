#include <stdlib.h>

void	*ft_malloc(size_t size);

int main(void)
{

	char *a = ft_malloc(16);
	(void)a;
	char *b = ft_malloc(40);
	(void)b;
	char *c = ft_malloc(30);
	(void)c;
	return 0;
}
