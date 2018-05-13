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

void		debug_nodes(void)
{
	struct s_node_page *e;
	int i;

	e = ctx.node_pages_entry;
	i = 0;
	while (i < ctx.node_pages_entry->primary_block.nb_node)
	{
		ft_printf("%.3i: ", i);
		ft_printf("{red}%s{eoc}", e->node[i].parent == NULL ? "ROOT " : "     ");
		if (e->node[i].mask.s.node_type == INDEX_TINY)
			ft_printf("UNIQUE: tiny_index_pages_tree, %p", &e->node[i]);
		else if (e->node[i].mask.s.node_type == INDEX_MEDIUM)
			ft_printf("UNIQUE: medium_index_pages_tree");
		else if (e->node[i].mask.s.node_type == RECORD_ALLOCATED_LARGE)
			ft_printf("UNIQUE: big_page_record_tree");
		else if (e->node[i].mask.s.node_type == PARENT_RECORD_FREE_TINY)
			ft_printf("UNIQUE: global_tiny_space_tree");
		else if (e->node[i].mask.s.node_type == PARENT_RECORD_FREE_MEDIUM)
			ft_printf("UNIQUE: global_medium_space_tree");
		else if (e->node[i].mask.s.node_type == RECORD_ALLOCATED_TINY)
			ft_printf("        record_allocated_tiny");
		else if (e->node[i].mask.s.node_type == RECORD_ALLOCATED_MEDIUM)
			ft_printf("        record_allocated_medium");
		else if (e->node[i].mask.s.node_type == RECORD_FREE_TINY)
			ft_printf("        record_free_tiny");
		else if (e->node[i].mask.s.node_type == RECORD_FREE_MEDIUM)
			ft_printf("        record_free_medium");
		else
			ft_printf("unknown node");
		ft_printf("\n");
		i++;
	}
}

static void	display_alloc(struct s_node *record)
{
	ft_printf("%p - %lu\n",
			record->content,
			record->size);
}

static void	display_pages(struct s_node *index)
{
	ft_printf("PAGE: %p\n", (void *)index->size);
	btree_apply_infix(
			(struct s_node *)index->content,
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
}
