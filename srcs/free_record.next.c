/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_record.next.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

void					assign_parent_free_tiny(
		void *content,
		struct s_node *node)
{
	size_t *size;

	size = (size_t *)content;
	node->m.size = *size;
	node->ptr_a = NULL;
	node->mask.s.node_type = PARENT_RECORD_FREE_TINY;
}

void					assign_parent_free_medium(
		void *content,
		struct s_node *node)
{
	size_t *size;

	size = (size_t *)content;
	node->m.size = *size;
	node->ptr_a = NULL;
	node->mask.s.node_type = PARENT_RECORD_FREE_MEDIUM;
}

int						check_index_destroy(
		void *addr,
		size_t size,
		enum e_page_type type)
{
	struct s_node *parent;

	parent = NULL;
	if (type == TINY && size == TINY_RANGE)
		parent = btree_get_node_by_content(
				ctx.global_tiny_space_tree,
				&size, &cmp_size_to_node_size);
	if (type == MEDIUM && size == MEDIUM_RANGE)
		parent = btree_get_node_by_content(
				ctx.global_medium_space_tree,
				&size, &cmp_size_to_node_size);
	if (parent)
	{
		destroy_index(
				btree_get_node_by_content(
						ctx.index_pages_tree,
						addr,
						&cmp_m_addr_to_node_m_addr));
		return (1);
	}
	return (0);
}
