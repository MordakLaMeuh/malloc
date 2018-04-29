/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctor.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"
#include "ctor.h"

#ifndef __APPLE__
# define getpagesize() (size_t)sysconf(_SC_PAGESIZE)
#endif

void		main_constructor(void)
{
	int ret;

	ctx.page_size = getpagesize();
	ret = getrlimit(RLIMIT_DATA, &ctx.mem_limit);
	if (ret < 0)
	{
		ft_putstr_fd("dyn_allocator cannot get RLIMIT_DATA", STDERR_FILENO);
		exit(1);
	}
	ctx.first_record_page = NULL;
	ctx.last_record_page = NULL;
	ctx.record_density = (ctx.page_size - sizeof(struct s_primary_record)) /
		sizeof(struct s_record);
	ctx.index_density = (ctx.page_size - sizeof(struct s_primary_index)) /
		sizeof(struct s_index);
}

void		main_destructor(void)
{
}
