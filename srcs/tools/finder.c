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

static int			cmp_record_addr_to_tiny_index(void *record, void *index)
{
	if (((struct s_record *)record)->addr < ((struct s_index *)index)->page_addr)
		return (-1);
	if (((struct s_record *)record)->addr >=
			(((struct s_index *)index)->page_addr + TINY_RANGE))
		return (1);
	return (0);
}

static int			cmp_record_addr_to_medium_index(void *record, void *index)
{
	if (((struct s_record *)record)->addr < ((struct s_index *)index)->page_addr)
		return (-1);
	if (((struct s_record *)record)->addr >=
			(((struct s_index *)index)->page_addr + MEDIUM_RANGE))
		return (1);
	return (0);
}

static int			cmp_record_to_record(void *record1, void *record2)
{
	if (((struct s_record *)record1)->addr < ((struct s_record *)record2)->addr)
		return (-1);
	if (((struct s_record *)record1)->addr > ((struct s_record *)record2)->addr)
		return (1);
	return (0);
}

static int		cmp_sub_size_to_recursive_record(void *size, void *record_node)
{
	struct s_record *record;
	uint64_t *length;

	length = (uint64_t *)size;
	record = btree_get_node_content(record_node);
	ft_printf("lenght = %lu, record_size = %lu\n", *length, record->size);
	if (*length < record->size)
		return (-1);
	if (*length > record->size)
		return (1);
	return (0);
}

struct s_node		*get_associated_node_for_record(struct s_record *record)
{
	struct s_index		*index;
	enum e_page_type	page_type;
	struct s_node		*node;

	page_type = get_page_type(record->size);
	if (page_type == LARGE)
	{
		return (btree_get_node_by_content(ctx.big_page_record_tree,
			record, &cmp_record_to_record));
	}
	if (page_type == TINY)
		index = (struct s_index *)btree_search_content(
		ctx.tiny_index_pages_tree, record, &cmp_record_addr_to_tiny_index);
	else
		index = (struct s_index *)btree_search_content(
		ctx.medium_index_pages_tree, record,
		&cmp_record_addr_to_medium_index);
	assert(index != NULL);
	node = btree_get_node_by_content(index->allocation_tree,
			record, &cmp_record_to_record);
	if (node)
		return (node);

	ft_printf("record addr = %p, size = %lu\n", record->addr, record->size);

	struct s_node *size_node_tree = btree_get_node_by_content(
			(page_type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&record->size, &cmp_sub_size_to_recursive_record);

	assert(size_node_tree != NULL);

	struct s_node *record_node = btree_get_node_content(size_node_tree);

	assert(record_node != NULL);

	node = btree_get_node_by_content(
			record_node,
			record, &cmp_record_to_record);
	if (node)
		return (node);


	return (NULL);
}

static int			cmp_index_to_index(void *index1, void *index2)
{
	if (((struct s_index *)index1)->page_addr < ((struct s_index *)index2)->page_addr)
		return (-1);
	if (((struct s_index *)index1)->page_addr > ((struct s_index *)index2)->page_addr)
		return (1);
	return (0);
}

struct s_node		*get_associated_node_for_index(struct s_index *index)
{
	if (index->type == TINY)
	{
		return (btree_get_node_by_content(ctx.tiny_index_pages_tree,
				index,
				&cmp_index_to_index));
	}
	else if (index->type == MEDIUM)
	{
		return (btree_get_node_by_content(ctx.medium_index_pages_tree,
				index,
				&cmp_index_to_index));
	}
	return (NULL);
}

static int			cmp_addr_to_record(void *addr, void *record)
{
	uint64_t *address;

	address = (uint64_t *)addr;
	if (*address < ((struct s_record *)record)->addr)
		return (-1);
	if (*address > ((struct s_record *)record)->addr)
		return (1);
	return (0);
}

static int			cmp_addr_to_tiny_index(void *addr, void *index)
{
	uint64_t *address;

	address = (uint64_t *)addr;
	if (*address < ((struct s_index *)index)->page_addr)
		return (-1);
	if (*address >= (((struct s_index *)index)->page_addr + TINY_RANGE))
		return (1);
	return (0);
}

static int			cmp_addr_to_medium_index(void *addr, void *index)
{
	uint64_t *address;

	address = (uint64_t *)addr;
	if (*address < ((struct s_index *)index)->page_addr)
		return (-1);
	if (*address >= (((struct s_index *)index)->page_addr + MEDIUM_RANGE))
		return (1);
	return (0);
}


struct s_node		*search_record_node(uint64_t addr, struct s_index **index)
{
	struct s_node			*node;

	*index = NULL;
	node = (struct s_node *)btree_get_node_by_content(ctx.big_page_record_tree,
					(void *)addr, &cmp_addr_to_record);
	if (node)
		return (node);
	*index = (struct s_index *)btree_get_node_by_content(
		ctx.medium_index_pages_tree, (void *)addr, &cmp_addr_to_medium_index);
	if (*index)
	{
		return ((struct s_node *)
				btree_get_node_by_content((*index)->allocation_tree,
				(void *)addr, &cmp_addr_to_record));
	}
	*index = (struct s_index *)btree_get_node_by_content(
		ctx.tiny_index_pages_tree, (void *)addr, &cmp_addr_to_tiny_index);
	if (*index)
	{
		return ((struct s_node *)
				btree_get_node_by_content((*index)->allocation_tree,
				(void *)addr, &cmp_addr_to_record));
	}
	return (NULL);
}
