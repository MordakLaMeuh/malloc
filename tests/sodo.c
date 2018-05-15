
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>

void	*ft_malloc(size_t size);
void	ft_free(void *ptr);
void	ft_show_alloc_mem(void);

#define TEST_LENGTH  10000
#define MAX_ALLOC 500

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
	tab_ptr[nb_elmt].ptr = ft_malloc(i);
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
	ft_free(tab_ptr[i].ptr);

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

	while (i < 10000000)
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

	ft_show_alloc_mem();

	i = 0;
	while (i < nb_elmt)
	{
		ft_free(tab_ptr[i].ptr);
		i++;
	}
	ft_show_alloc_mem();
}

int			main(void)
{
	sodo_test();
	return 0;
}
