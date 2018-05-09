#include <stdlib.h>

void	*ft_malloc(size_t size);

int main(void)
{
	char *a = ft_malloc(16);
	(void)a;
	return 0;
}
