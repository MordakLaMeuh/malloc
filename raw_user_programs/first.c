
#include <stdlib.h>

#include "libft_alloc.h"

#include <string.h>

#define NB_I 3000
#define NB_F 1500
#define NB_U 6000

#include <malloc.h>
#include <stdio.h>

int					ft_printf(const char *restrict format, ...);

int main(void) {
	char *h = memalign(512, 1024);
	printf("h pointe sur %p\n", h);
	char *w = memalign(512, 1024);
	printf("w pointe sur %p\n", w);
	char *z = memalign(512, 1024);
	printf("z pointe sur %p\n", z);

	h = ft_malloc(1024);
	printf("h pointe sur %p\n", h);
	w = ft_malloc(1024);
	printf("w pointe sur %p\n", w);
	z = ft_malloc(1024);
	printf("w pointe sur %p\n", z);

	ft_free(h);
	ft_free(w);
	ft_free(z);

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

	ft_debug_allocator();

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


	char *c;
	c = ft_malloc(100);
	c = ft_realloc(c, 120);
	c = ft_realloc(c, 150);
	c = ft_realloc(c, 250);
	c = ft_realloc(c, 400);
	x = ft_malloc(200);
	t[0] = ft_malloc(1024);
	t[1] = ft_malloc(1024);
	t[2] = ft_malloc(1024);
	t[3] = ft_malloc(1024);
	ft_debug_allocator();
	c = ft_realloc(c, 300);
	c = ft_realloc(c, 400);
	c = ft_realloc(c, 450);
	ft_free(c);
	ft_free(x);
	ft_debug_allocator();
	ft_free(t[1]);
	ft_free(t[2]);
	ft_free(t[3]);
	ft_free(t[0]);
	c = ft_realloc(NULL, 65536);
	ft_debug_allocator();
	return 0;
}
