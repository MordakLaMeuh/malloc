/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   class.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "dyn_allocator.h"

void	before_start(void)
{
	ctx.page_size = (size_t)getpagesize();
	ctx.first_idx_page = NULL;
	ctx.first_reg_page = NULL;
	printf("--- DEBUG SUMMARY ---\n");
	printf("page_size = %lu\n", ctx.page_size);
	printf("size of idx_field = %lu\n", sizeof(struct s_idx_page_description));
	printf("size of pri_idx = %lu\n", sizeof(struct s_primary_idx_block));
	printf("size of reg_field = %lu\n", sizeof(struct s_reg));
	printf("size of pri_reg = %lu\n", sizeof(struct s_primary_reg_block));
	printf("---------------------\n");
}
