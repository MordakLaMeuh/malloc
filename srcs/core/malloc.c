/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

void			*malloc(size_t size)
{
	ft_putstr("custom malloc called !\n");
	(void)size;
	ctx.first_idx_page = mmap(NULL, ctx.page_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	struct s_idx_page *idx_page = ctx.first_idx_page;
	idx_page->primary_block.next_idx_page = NULL;
	struct s_idx_page_description *desc = &idx_page->page_desc_field[0];
	desc->type = TINY;
	desc->mask_sector_a |= 1;
	desc->page = mmap(NULL, ctx.page_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	desc->allocated = ALLOCATED;
	return (desc->page);
}
