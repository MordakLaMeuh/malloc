/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_chunk.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static int				cmp_index_to_index(void *index1, void *index2)
{
	if (((struct s_index *)index1)->page_addr < ((struct s_index *)index2)->page_addr)
		return (-1);
	if (((struct s_index *)index1)->page_addr > ((struct s_index *)index2)->page_addr)
		return (1);
	return (0);
}
static struct s_node	*assign_new_tiny_chunk(
		struct s_record *record,
		struct s_index *index_ref,
		struct s_node **free_record_sub_tree)
{
	ft_printf("Assigning new TINY chunk\n");
	record->size = TINY_RANGE;
	if (free_record_sub_tree)
	{
		ft_printf("free_record_sub_tree\n");
		*free_record_sub_tree = add_free_record(record, TINY);
	}
	else
		add_free_record(record, TINY);
	return (btree_insert_rnb_node_by_content(&ctx.tiny_index_pages_tree,
			index_ref, &cmp_index_to_index, &node_custom_allocator));
}

static struct s_node	*assign_new_medium_chunk(
		struct s_record *record,
		struct s_index *index_ref,
		struct s_node **free_record_sub_tree)
{
	ft_printf("Assigning new MEDIUM chunk\n");
	record->size = MEDIUM_RANGE;
	if (free_record_sub_tree)
		*free_record_sub_tree = add_free_record(record, MEDIUM);
	else
		add_free_record(record, MEDIUM);
	return (btree_insert_rnb_node_by_content(&ctx.medium_index_pages_tree,
			index_ref, &cmp_index_to_index, &node_custom_allocator));
}

/*
** Assign a new chunk after a system page request.
** Create a new node of index
** Create a new node of free space
** Return the reference of the index.
*/

struct s_index			*assign_new_chunk(
		void *addr,
		enum e_page_type page_type,
		struct s_node **free_record_sub_tree)
{
	struct s_index		*index_ref;
	struct s_record		*record;
	struct s_node		*node;

	index_ref = index_custom_allocator();
	if (index_ref == NULL)
		return (NULL);
	index_ref->page_addr = (uint64_t)addr;
	index_ref->allocation_tree = btree_new();
	index_ref->type = page_type;
	record = record_custom_allocator();
	if (record == NULL)
		return (NULL);
	record->addr = (uint64_t)addr;
	if (index_ref->type == TINY)
		node = assign_new_tiny_chunk(record, index_ref, free_record_sub_tree);
	else
		node = assign_new_medium_chunk(record, index_ref, free_record_sub_tree);
	return ((node != NULL) ? index_ref : NULL);
}

/*
** Tell the system to allocate new pages.
*/

struct s_index			*reserve_new_chunk(
		enum e_page_type page_type,
		struct s_node **free_record_sub_tree)
{
	void				*addr;

	if (page_type == TINY)
		addr = get_new_pages(TINY_RANGE);
	else
		addr = get_new_pages(MEDIUM_RANGE);
	if (addr == NULL)
		return (NULL);
	return (assign_new_chunk(addr, page_type, free_record_sub_tree));
}

