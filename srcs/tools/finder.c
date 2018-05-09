/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

struct s_node	*get_associated_node_for_record(struct s_record *record)
{
	struct s_index		*index;
	enum e_page_type	page_type;
	struct s_node		*node;

	page_type = get_page_type(record->size);
	if (page_type == LARGE)
	{
		return (btree_get_node_by_content(ctx.big_page_record_tree,
			(void *)record->addr, &cmp_index_addr));
	}
	if (page_type == TINY)
		index = (struct s_index *)btree_search_content(
		ctx.tiny_index_pages_tree, (void *)record->addr, &cmp_tiny_addr_range);
	else
		index = (struct s_index *)btree_search_content(
		ctx.medium_index_pages_tree, (void *)record->addr,
		&cmp_medium_addr_range);
	assert(index != NULL);
	node = btree_get_node_by_content(index->allocation_tree,
			(void *)record->addr, &cmp_record_addr);
	if (node)
		return (node);
	return (NULL);
}

struct s_node	*get_associated_node_for_index(struct s_index *index)
{
	if (index->type == TINY)
	{
		return (btree_get_node_by_content(ctx.tiny_index_pages_tree,
				index,
				&cmp_index_addr));
	}
	else if (index->type == MEDIUM)
	{
		return (btree_get_node_by_content(ctx.medium_index_pages_tree,
				index,
				&cmp_index_addr));
	}
	return (NULL);
}

/*
** Search a record.
*/

struct s_node	*search_record_node(uint64_t addr, struct s_index **index)
{
	struct s_node			*node;

	*index = NULL;
	node = (struct s_node *)btree_get_node_by_content(ctx.big_page_record_tree,
					(void *)addr, &cmp_index_addr);
	if (node)
		return (node);
	*index = (struct s_index *)btree_get_node_by_content(
		ctx.medium_index_pages_tree, (void *)addr, &cmp_medium_addr_range);
	if (*index)
	{
		return ((struct s_node *)
				btree_get_node_by_content((*index)->allocation_tree,
				(void *)addr, &cmp_record_addr));
	}
	*index = (struct s_index *)btree_get_node_by_content(
		ctx.tiny_index_pages_tree, (void *)addr, &cmp_tiny_addr_range);
	if (*index)
	{
		return ((struct s_node *)
				btree_get_node_by_content((*index)->allocation_tree,
				(void *)addr, &cmp_record_addr));
	}
	return (NULL);
}
