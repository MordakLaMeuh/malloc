/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deep.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sodo.h"

void			add_sodo(
		struct s_test tab_ptr[TEST_LENGTH],
		int nb_elmt)
{
	int i;

	i = rand() % (MAX_ALLOC);
	tab_ptr[nb_elmt].c = i % 256;
	tab_ptr[nb_elmt].ptr = malloc(i);
	tab_ptr[nb_elmt].size = (size_t)i;
	ft_memset(tab_ptr[nb_elmt].ptr, tab_ptr[nb_elmt].c, i);
}

void			del_sodo(
		struct s_test tab_ptr[TEST_LENGTH],
		int nb_elmt)
{
	int		i;
	size_t	n;
	uint8_t	*ptr;

	n = 0;
	i = rand() % nb_elmt;
	ptr = (uint8_t *)tab_ptr[i].ptr;
	while (n < tab_ptr[i].size)
	{
		if (*ptr != tab_ptr[i].c)
		{
			ft_printf("BAD VALUE: Got %hhx instead of %hhx\n",
					*ptr, tab_ptr[i].c);
			exit(1);
		}
		ptr++;
		n++;
	}
	free(tab_ptr[i].ptr);
	if (i != (nb_elmt - 1))
		tab_ptr[i] = tab_ptr[nb_elmt - 1];
}

static void		real_sodo_next(
		struct s_test tab_ptr[TEST_LENGTH],
		size_t x,
		uint8_t *ptr,
		int i)
{
	size_t			n;
	size_t			n_size;

	if ((tab_ptr[i].ptr = realloc(tab_ptr[i].ptr, x)) == NULL)
	{
		ft_printf("BAD REALLOC\n");
		exit(1);
	}
	n = 0;
	ptr = (uint8_t *)tab_ptr[i].ptr;
	n_size = (tab_ptr[i].size < x) ? tab_ptr[i].size : x;
	while (n < n_size)
	{
		if (*ptr != tab_ptr[i].c)
		{
			ft_printf("BAD VALUE: Got %hhx instead of %hhx\n",
					*ptr, tab_ptr[i].c);
			exit(1);
		}
		ptr++;
		n++;
	}
	tab_ptr[i].size = (size_t)x;
	tab_ptr[i].c = x % 256;
	ft_memset(tab_ptr[i].ptr, tab_ptr[i].c, x);
}

void			real_sodo(
		struct s_test tab_ptr[TEST_LENGTH],
		int *nb_elmt)
{
	uint8_t			*ptr;
	size_t			n;
	size_t			x;
	int				i;

	n = 0;
	i = rand() % *nb_elmt;
	ptr = (uint8_t *)tab_ptr[i].ptr;
	while (n < tab_ptr[i].size)
	{
		if (*ptr != tab_ptr[i].c)
		{
			ft_printf("BAD VALUE: Got %hhx instead of %hhx\n",
					*ptr, tab_ptr[i].c);
			exit(1);
		}
		ptr++;
		n++;
	}
	x = rand() % (MAX_ALLOC);
	if (ptr == NULL || x == 0)
		return ;
	real_sodo_next(tab_ptr, x, ptr, i);
}
