
#include <string.h>

void	*ft_malloc(size_t size);
void	ft_show_alloc_mem(void);

int main(void)
{
	for (int i = 0; i < 1024; i++) {
		char *i = ft_malloc(1024);
	}
	ft_show_alloc_mem();
}
