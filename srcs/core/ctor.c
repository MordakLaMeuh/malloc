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

#ifdef DEBUG_CTOR
#include <limits.h>
#endif

void _constructor(void)
{
    int ret;
#ifdef DEBUG_CTOR
	printf("Constructor called\n");
#endif
#ifdef __APPLE__
	ctx.page_size = getpagesize();
#else
    long res;

    res = (size_t)sysconf(_SC_PAGESIZE);
    if (res < 0) {
        ft_putstr_fd("dyn_allocator cannot page size", STDERR_FILENO);
        exit (1);
    }
    ctx.page_size = (size_t)res;
#endif
    ret = getrlimit(RLIMIT_DATA, &ctx.mem_limit);
    if (ret < 0) {
        ft_putstr_fd("dyn_allocator cannot get RLIMIT_DATA", STDERR_FILENO);
        exit (1);
    }
	ctx.first_record_page = NULL;
	ctx.last_record_page = NULL;
	ctx.record_density =
		(ctx.page_size - sizeof(struct s_primary_record)) /
		sizeof(struct s_record);
	ctx.index_density =
		(ctx.page_size - sizeof(struct s_primary_index)) /
		sizeof(struct s_index);
#ifdef DEBUG_CTOR
	printf("--- DEBUG SUMMARY ---\n");
	printf("page_size: %lu\n", ctx.page_size);
    printf("RLIMIT_DATA -> current: %lu, max: %lu\n",
        (ctx.mem_limit.rlim_cur == ULONG_MAX) ? 0 : ctx.mem_limit.rlim_cur,
        (ctx.mem_limit.rlim_max == ULONG_MAX) ? 0 : ctx.mem_limit.rlim_max);
	printf("size of record block: %lu\n", sizeof(struct s_record));
	printf("size of pri_record: %lu\n", sizeof(struct s_primary_record));
	printf("record per page: %i, trash: %lu\n",
		ctx.record_density,
		ctx.page_size -
		(ctx.record_density * sizeof(struct s_record) +
		sizeof(struct s_primary_record)));
	printf("index per page: %i, trash: %lu\n",
		ctx.index_density,
		ctx.page_size -
		(ctx.index_density * sizeof(struct s_index) +
		sizeof(struct s_primary_index)));
	printf("---------------------\n");
#endif
}

void _destructor(void)
{
#ifdef DEBUG_CTOR
	printf("Destructor called\n");
#endif
}
