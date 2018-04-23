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

void _constructor(void)
{
#ifdef DEBUG_INFO
	printf("Constructor called\n");
#endif
#ifdef __APPLE__
	ctx.page_size = getpagesize();
#else
    long res;

    res = (size_t)sysconf(_SC_PAGESIZE);
    if (res < 0) {
        perror("dyn_allocator constructor");
        exit (1);
    }
    ctx.page_size = (size_t)res;
#endif
	ctx.first_idx_page = NULL;
	ctx.first_reg_page = NULL;
#ifdef DEBUG_INFO
	printf("--- DEBUG SUMMARY ---\n");
	printf("page_size = %lu\n", ctx.page_size);
	printf("size of idx_field = %lu\n", sizeof(struct s_idx_page_description));
	printf("size of pri_idx = %lu\n", sizeof(struct s_primary_idx_block));
	printf("size of reg_field = %lu\n", sizeof(struct s_reg));
	printf("size of pri_reg = %lu\n", sizeof(struct s_primary_reg_block));
	printf("---------------------\n");
#endif
}

void _destructor(void)
{
#ifdef DEBUG_INFO
	printf("Destructor called\n");
#endif
}
