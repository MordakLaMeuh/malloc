
#include <stdlib.h>

#include "libft_alloc.h"

#define NB_I 3000
#define NB_F 1500
#define NB_U 6000

int main(void) {
	char *t[65636];
	int i;
	for (i = 0; i < NB_I; i++)
		t[i] = ft_malloc(10);

	for (i = 0; i < NB_F; i++)
		ft_free(t[i]);

	for (i = 0; i < NB_F; i++)
		t[i] = ft_malloc(10);

	for (i = NB_I; i < NB_U; i++)
		t[i] = ft_malloc(10);

	for (i = 0; i < NB_U; i++)
		ft_free(t[i]);

	char *x = ft_malloc(10);
	char *y = ft_malloc(10);
	ft_free(x);
	ft_free(y);
	ft_free(y);
	return 0;
}
