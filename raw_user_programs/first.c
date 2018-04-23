
#include <stdlib.h>

#include "libft_alloc.h"

int main(void) {
	char *s = ft_malloc(10);
	char *t = ft_malloc(10);
	ft_free(s);
	ft_free(t);
	return 0;
}
