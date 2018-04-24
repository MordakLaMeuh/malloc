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

#ifndef __APPLE__
#include <stdlib.h>
#include <errno.h>
#endif
#include <unistd.h>
#include "dyn_allocator.h"

#ifdef DEBUG_INFO
#include <limits.h>
#endif

void _constructor(void)
{
    int ret;
#ifdef DEBUG_INFO
	printf("Constructor called\n");
#endif
#ifdef __APPLE__
	ctx.page_size = getpagesize();
#else
    long res;

    res = (size_t)sysconf(_SC_PAGESIZE);
    if (res < 0) {
        perror("dyn_allocator cannot page size");
        exit (1);
    }
    ctx.page_size = (size_t)res;
#endif
    ret = getrlimit(RLIMIT_DATA, &ctx.mem_limit);
    if (ret < 0) {
        perror("dyn_allocator cannot get RLIMIT_DATA");
        exit (1);
    }
	ctx.first_record_page = NULL;
	ctx.last_record_page = NULL;
	ctx.record_density =
		(ctx.page_size - sizeof(struct s_primary_record)) /
		sizeof(struct s_record);
#ifdef DEBUG_INFO
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

	printf("size of idx_field: %lu\n", sizeof(struct s_idx_page_description));
	printf("size of pri_idx: %lu\n", sizeof(struct s_primary_idx_block));
	printf("---------------------\n");
#endif
}

void _destructor(void)
{
#ifdef DEBUG_INFO
	printf("Destructor called\n");
#endif
}
