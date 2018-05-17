/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sodo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sodo.h"

static void		loop_sodo_test(
		struct s_test tab_ptr[TEST_LENGTH],
		int global_count[2],
		int *nb_elmt)
{
	int		op;
	int		i;

	srand(get_timestamp());
	i = 0;
	while (i < NB_TESTS)
	{
		op = rand();
		if (*nb_elmt == 0 || ((op & 0x1) == 0 && *nb_elmt < TEST_LENGTH))
		{
			add_sodo(tab_ptr, *nb_elmt);
			*nb_elmt += 1;
			global_count[0] += 1;
		}
		else
		{
			del_sodo(tab_ptr, *nb_elmt);
			*nb_elmt -= 1;
			global_count[1] += 1;
		}
		i++;
	}
}

void			sodo_test(struct s_test	tab_ptr[TEST_LENGTH])
{
	int				nb_elmt;
	int				global_count[2];
	int				i;

	srand(get_timestamp());
	nb_elmt = 0;
	bzero(global_count, 2 * sizeof(int));
	loop_sodo_test(tab_ptr, global_count, &nb_elmt);
	printf("%i malloc made,  %i free made\n", global_count[0], global_count[1]);
	show_alloc_mem();
	i = 0;
	while (i < nb_elmt)
	{
		free(tab_ptr[i].ptr);
		i++;
	}
	show_alloc_mem();
}

static void		loop_sodo_realloc(
		struct s_test tab_ptr[TEST_LENGTH],
		int global_count[2],
		int *nb_elmt)
{
	int			op;
	int			i;

	i = -1;
	while (++i < NB_TESTS)
	{
		op = rand() % 3;
		if (*nb_elmt == 0 || (op == 0 && *nb_elmt < TEST_LENGTH))
		{
			add_sodo(tab_ptr, *nb_elmt);
			*nb_elmt += 1;
			global_count[0] += 1;
		}
		else if (op == 1)
		{
			del_sodo(tab_ptr, *nb_elmt);
			*nb_elmt -= 1;
			global_count[1] += 1;
		}
		else
		{
			real_sodo(tab_ptr, nb_elmt);
			global_count[2] += 1;
		}
	}
}

void			sodo_realloc(struct s_test tab_ptr[TEST_LENGTH])
{
	int				nb_elmt;
	int				global_count[3];
	int				i;

	srand(get_timestamp());
	nb_elmt = 0;
	bzero(global_count, 3 * sizeof(int));
	loop_sodo_realloc(tab_ptr, global_count, &nb_elmt);
	printf("%i realloc made, %i mallocs and %i free made\n",
			global_count[2], global_count[0], global_count[1]);
	show_alloc_mem();
	i = 0;
	if (nb_elmt != 0)
		while (i < nb_elmt - 1)
		{
			free(tab_ptr[i].ptr);
			i++;
		}
	show_alloc_mem();
}

int				main(void)
{
	struct s_test	tab_ptr[TEST_LENGTH];
	char			*abs;

	abs = malloc(10);
	free(abs - 8000000);
	free(abs);
	sodo_test(tab_ptr);
	sodo_realloc(tab_ptr);
	return (0);
}
