
#include <stdlib.h>

#include "libft_alloc.h"

#include <string.h>

#define NB_I 3000
#define NB_F 1500
#define NB_U 6000

int main(void) {
	char *t[65636];
	int i;
	for (i = 0; i < NB_I; i++) {
		t[i] = ft_malloc(50);
		strcpy(t[i], "les carotes sont cuites");
		if (strcmp(t[i], "les carotes sont cuites") != 0)
			exit (1);
	}

	for (i = 0; i < NB_F; i++)
		ft_free(t[i]);

	for (i = 0; i < NB_F; i++)
		t[i] = ft_malloc(700);

	for (i = NB_I; i < NB_U; i++) {
		t[i] = ft_malloc(700);
		strcpy(t[i], "les carotes sont cuites");
		if (strcmp(t[i], "les carotes sont cuites") != 0)
			exit (1);
	}

	for (i = 0; i < NB_U; i++)
		ft_free(t[i]);

	char *x = ft_malloc(10);
	char *y = ft_malloc(10);
	ft_free(x);
	ft_free(y);


	ft_debug_allocator();
	char *c;
	c = ft_malloc(100);
	c = ft_realloc(c, 120);
	c = ft_realloc(c, 150);
	c = ft_realloc(c, 250);
	c = ft_realloc(c, 400);
	x = ft_malloc(200);
	c = ft_realloc(c, 300);
	c = ft_realloc(c, 400);
	c = ft_realloc(c, 450);
	ft_free(c);
	ft_free(x);
	ft_debug_allocator();
	return 0;
}
