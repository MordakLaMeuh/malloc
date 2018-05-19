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

/*
** The aim of this function is to avoid having two full free pages:
** Regarding to TINY_RANGE or MEDIUM_RANGE, this function destroy
** a index page if his record is totally free and a total free record
** exists already.
** Return 1 if the suppress operation has been done. In this case,
** 			no new free chunk will be created.
** Return 0 if there are no free node like that, it will be
** 			created in caller.
*/

int						check_index_destroy(
		void *addr,
		size_t size,
		enum e_page_type type)
{
	struct s_node *parent;
	struct s_node *index;

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
		index = btree_get_node_by_content(
				ctx.index_pages_tree,
				addr,
				&cmp_m_addr_to_node_m_addr);
		destroy_index(index);
		destroy_pages(addr, size);
		return (1);
	}
	return (0);
}
