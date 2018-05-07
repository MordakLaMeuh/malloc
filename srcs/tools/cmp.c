/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

int				cmp_index_addr(void *i1, void *i2)
{
	if (((struct s_index *)i1)->page_addr < ((struct s_index *)i2)->page_addr)
		return (-1);
	if (((struct s_index *)i1)->page_addr > ((struct s_index *)i2)->page_addr)
		return (1);
	return (0);
}

int				cmp_record_size(void *r1, void *r2)
{
	if (((struct s_record *)r1)->size < ((struct s_record *)r2)->size)
		return (-1);
	if (((struct s_record *)r1)->size > ((struct s_record *)r2)->size)
		return (1);
	return (0);
}

int				cmp_record_addr(void *r1, void *r2)
{
	if (((struct s_record *)r1)->addr < ((struct s_record *)r2)->addr)
		return (-1);
	if (((struct s_record *)r1)->addr > ((struct s_record *)r2)->addr)
		return (1);
	return (0);
}

int				cmp_tiny_addr_range(void *r1, void *r2)
{
	if (((struct s_record *)r1)->addr < ((struct s_record *)r2)->addr)
		return (-1);
	if (((struct s_record *)r1)->addr >=
			(((struct s_record *)r2)->addr + TINY_RANGE))
		return (1);
	return (0);
}

int				cmp_medium_addr_range(void *r1, void *r2)
{
	if (((struct s_record *)r1)->addr < ((struct s_record *)r2)->addr)
		return (-1);
	if (((struct s_record *)r1)->addr >=
			(((struct s_record *)r2)->addr + MEDIUM_RANGE))
		return (1);
	return (0);
}
