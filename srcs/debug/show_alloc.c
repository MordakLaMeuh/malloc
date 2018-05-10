/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

void		display_alloc(void *index, int lvl, int first_elem)
{
	(void)first_elem;
	(void)lvl;
	ft_printf("%p - %p\n", ((struct s_record *)index)->addr);
}

void		display_pages(void *index, int lvl, int first_elem)
{
	(void)first_elem;
	(void)lvl;
	ft_printf("PAGE: %p", ((struct s_index *)index)->page_addr);
	btree_apply_by_level(((struct s_index *)index)->allocation_tree, &display_alloc);
}

void		show_alloc(void)
{
	ft_printf("--- TINY ---\n");
	btree_apply_by_level(ctx.tiny_index_pages_tree, &display_pages);
	ft_printf("\n--- MEDIUM ---\n");
	btree_apply_by_level(ctx.medium_index_pages_tree, &display_pages);
}
