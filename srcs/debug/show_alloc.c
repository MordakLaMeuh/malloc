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

void		find_root(void *tmp);

static void	display_alloc(void *index)
{
	ft_printf("%p - %lu\n",
			((struct s_record *)index)->addr,
			((struct s_record *)index)->size);
}

static void	display_pages(void *index)
{
	ft_printf("PAGE: %p\n", ((struct s_index *)index)->page_addr);
	btree_apply_infix(
			((struct s_index *)index)->allocation_tree,
			&display_alloc);
}

void		show_alloc(void)
{
	ft_printf("* * * TINY * * *\n");
	btree_apply_infix(ctx.tiny_index_pages_tree, &display_pages);
	ft_printf("\n* * * MEDIUM * * *\n");
	btree_apply_infix(ctx.medium_index_pages_tree, &display_pages);
	ft_printf("\n* * * LARGE * * *\n");
	btree_apply_infix(ctx.big_page_record_tree, &display_alloc);
	find_root(NULL);
}
