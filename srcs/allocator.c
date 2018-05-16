/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static void			*core_allocator_large(
		size_t *size)
{
	void			*addr;
	struct s_node	*record;

	addr = get_new_pages(*size);
	if (addr == NULL)
		return (NULL);
	record = btree_create_node(&node_custom_allocator);
	if (record == NULL)
	{
		destroy_pages(addr, *size);
		return (NULL);
	}
	record->ptr_a = addr;
	record->m.size = *size;
	record->mask.s.node_type = RECORD_ALLOCATED_LARGE;
	record = btree_insert_rnb_node(&ctx.big_page_record_tree,
			record, &cmp_node_addr_to_node_addr);
	if (record == NULL)
	{
		destroy_pages(addr, *size);
		btree_destroy_node(record, &node_custom_deallocator);
		return (NULL);
	}
	return (addr);
}

static void			*core_allocator_tiny_medium(
		size_t *size,
		enum e_page_type type)
{
	struct s_node	*free_parent_tree;
	struct s_node	*free_record;
	struct s_node	*record;
	size_t			free_size;
	uint64_t		addr;

	if ((free_parent_tree = get_best_free_record_tree(*size, type)) == NULL)
		return (NULL);
	free_record = free_parent_tree->ptr_a;
	free_size = free_record->m.size;
	addr = (uint64_t)free_record->ptr_a;
	delete_free_record(free_record, free_parent_tree, type);
	if (free_size > *size)
	{
		insert_free_record((void *)(addr + (uint64_t)*size),
				free_size - *size, type, NULL);
	}
	if ((record = btree_create_node(&node_custom_allocator)) == NULL)
		return (NULL);
	record->m.size = *size;
	record->ptr_a = (void *)addr;
	record->mask.s.node_type = (type == TINY) ?
			RECORD_ALLOCATED_TINY : RECORD_ALLOCATED_MEDIUM;
	return (insert_allocated_record(record));
}

void				*core_allocator(size_t *size)
{
	enum e_page_type	page_type;
	void				*addr;

	if (*size == 0)
		return (NULL);
	page_type = get_page_type(*size);
	*size = allign_size(*size, page_type);
	if (page_type != LARGE)
		addr = core_allocator_tiny_medium(size, page_type);
	else
		addr = core_allocator_large(size);
	if (addr == NULL)
	{
		ft_printf("%s ENOMEM: %lu\n", __func__, size);
		ft_show_alloc_mem();
		errno = ENOMEM;
	}
	return (addr);
}
