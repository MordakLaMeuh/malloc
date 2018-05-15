
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>

void	*malloc(size_t size);
void	free(void *ptr);
void	show_alloc_mem(void);
void	*realloc(void *ptr, size_t size);

#define TEST_LENGTH  100000
#define MAX_ALLOC 400

#define NB_TESTS 1000000

struct test {
	void *ptr;
	uint8_t c;
	size_t size;
};

uint64_t		GetTimeStamp(void) {
	struct timeval tv;

    gettimeofday(&tv,NULL);
    return tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
}

static void	add_sodo(
		struct test tab_ptr[TEST_LENGTH],
		int nb_elmt)
{
	int i = rand() % (MAX_ALLOC);
	tab_ptr[nb_elmt].c = i % 256;
	tab_ptr[nb_elmt].ptr = malloc(i);
	tab_ptr[nb_elmt].size = (size_t)i;
	memset(tab_ptr[nb_elmt].ptr, tab_ptr[nb_elmt].c, i);
}

static void	del_sodo(
		struct test tab_ptr[TEST_LENGTH],
		int nb_elmt)
{
	int i;
	size_t n = 0;

	i = rand() % nb_elmt;
	uint8_t *ptr = (uint8_t *)tab_ptr[i].ptr;
	while (n < tab_ptr[i].size)
	{
		if (*ptr != tab_ptr[i].c)
		{
			printf("BAD VALUE: Got %hhx instead of %hhx\n", *ptr, tab_ptr[i].c);
			exit (1);
		}
		ptr++;
		n++;
	}
	free(tab_ptr[i].ptr);

	if (i != (nb_elmt - 1))
		tab_ptr[i] = tab_ptr[nb_elmt - 1];
}

void		sodo_test(void)
{
	srand(GetTimeStamp());
	struct test tab_ptr[TEST_LENGTH];

	int nb_elmt = 0;
	int count_add = 0;
	int count_del = 0;
	int i = 0;

	while (i < NB_TESTS)
	{
		int op = rand();
		if (nb_elmt == 0 || ((op & 0x1) == 0 && nb_elmt < TEST_LENGTH)) {
			add_sodo(tab_ptr, nb_elmt);
			nb_elmt++;
			count_add++;
		} else {
			del_sodo(tab_ptr, nb_elmt);
			nb_elmt--;
			count_del++;
		}
		i++;
	}
	printf("%i malloc made and %i free made\n", count_add, count_del);

	show_alloc_mem();

	i = 0;
	while (i < nb_elmt)
	{
		free(tab_ptr[i].ptr);
		i++;
	}
	show_alloc_mem();
}

static void	real_sodo(
		struct test tab_ptr[TEST_LENGTH],
		int *nb_elmt)
{
	int i;
	size_t n = 0;

	i = rand() % *nb_elmt;
	uint8_t *ptr = (uint8_t *)tab_ptr[i].ptr;
	while (n < tab_ptr[i].size)
	{
		if (*ptr != tab_ptr[i].c)
		{
			printf("BAD VALUE: Got %hhx instead of %hhx\n", *ptr, tab_ptr[i].c);
			exit (1);
		}
		ptr++;
		n++;
	}
	int x = rand() % (MAX_ALLOC);
	if (ptr == NULL || x == 0)
		return ;


	tab_ptr[i].ptr = realloc(tab_ptr[i].ptr, x);
	if (tab_ptr[i].ptr == NULL) {
		printf("BAD REALLOC\n");
		exit (1);
	}

	n = 0;
	ptr = (uint8_t *)tab_ptr[i].ptr;
	size_t n_size = (tab_ptr[i].size < x) ? tab_ptr[i].size : x;

	while (n < n_size)
	{
		if (*ptr != tab_ptr[i].c)
		{
			printf("BAD VALUE: Got %hhx instead of %hhx\n", *ptr, tab_ptr[i].c);
			exit (1);
		}
		ptr++;
		n++;
	}

	tab_ptr[i].size = (size_t)x;
	tab_ptr[i].c = x % 256;
	memset(tab_ptr[i].ptr, tab_ptr[i].c, x);
}

void		sodo_realloc(void)
{
	srand(GetTimeStamp());
	struct test tab_ptr[TEST_LENGTH];

	int nb_elmt = 0;
	int count_add = 0;
	int count_del = 0;
	int count_realloc = 0;
	int i = 0;

	while (i < NB_TESTS)
	{
		int op = rand() % 3;
		if (nb_elmt == 0 || (op == 0 && nb_elmt < TEST_LENGTH)) {
			add_sodo(tab_ptr, nb_elmt);
			nb_elmt++;
			count_add++;
		} else if (op == 1) {
			del_sodo(tab_ptr, nb_elmt);
			nb_elmt--;
			count_del++;
		} else {
			real_sodo(tab_ptr, &nb_elmt);
			count_realloc++;
		}
		i++;
	}
	printf("%i realloc made, %i mallocs and %i free made\n", count_realloc, count_add, count_del);

	show_alloc_mem();

	i = 0;
	while (i < nb_elmt)
	{
		free(tab_ptr[i].ptr);
		i++;
	}
	show_alloc_mem();
}

int			main(void)
{
	sodo_test();
	sodo_realloc();
	return 0;
}
