
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

uint64_t		GetTimeStamp(void) {
	struct timeval tv;

    gettimeofday(&tv,NULL);
    return tv.tv_sec * (uint64_t)1000000 + tv.tv_usec;
}

static void	add_sodo(
		void *tab_ptr[TEST_LENGTH],
		int nb_elmt)
{
	int i = rand() % (MAX_ALLOC);
	tab_ptr[nb_elmt] = ft_malloc(i);
}

static void	del_sodo(
		void *tab_ptr[TEST_LENGTH],
		int nb_elmt)
{
	int i;

	i = rand() % nb_elmt;

	ft_free(tab_ptr[i]);

	if (i != (nb_elmt - 1))
		tab_ptr[i] = tab_ptr[nb_elmt - 1];
}

void		sodo_test(void)
{
	srand(GetTimeStamp());
	void *tab_ptr[TEST_LENGTH];

	int nb_elmt = 0;
	int count_add = 0;
	int count_del = 0;
	int i = 0;

	while (i < 100000)
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
		ft_free(tab_ptr[i]);
		i++;
	}
	ft_show_alloc_mem();
}

int			main(void)
{
	sodo_test();
	return 0;
}
