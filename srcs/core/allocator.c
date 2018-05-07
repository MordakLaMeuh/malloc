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

static uint64_t		find_index_and_set(
		size_t size,
		enum e_page_type page_type,
		struct s_index **index
)
{
	(void)size;
	(void)page_type;
	(void)index;
	return (0);
}

static void			*core_allocator_large(size_t *size)
{
	void			*addr;
	struct s_record	*record;
	struct s_node	*node;

	*size = allign_size(*size, LARGE);
	addr = get_new_pages(*size);
	if (addr == NULL)
		return (NULL);
	record = record_custom_allocator();
	if (record == NULL)
	{
		destroy_pages(addr, *size);
		return (NULL);
	}
	node = btree_insert_rnb_node_by_content(&ctx.big_page_record_tree,
			record, &cmp_record_addr, &node_custom_allocator);
	if (node == NULL)
	{
		destroy_pages(addr, *size);
		record_custom_deallocator(record);
		return (NULL);
	}
	record->size = *size;
	record->addr = (uint64_t)addr;
	return (addr);
}

static void			*core_allocator_tiny_medium(size_t *size)
{
	struct s_record		*record;
	struct s_index		*index;
	enum e_page_type	page_type;

	page_type = (*size <= TINY_LIMIT) ? TINY : MEDIUM;
	*size = allign_size(*size, page_type);
	if ((record = record_custom_allocator()) == NULL)
		return (NULL);
	record->size = *size;
	record->addr = find_index_and_set(*size, page_type, &index);
	if ((void *)record->addr == NULL)
	{
		record_custom_deallocator(record);
		return (NULL);
	}
	if (btree_insert_rnb_node_by_content(&index->allocation_tree,
			record, &cmp_record_addr, &node_custom_allocator) == NULL)
	{
		record_custom_deallocator(record);
		return (NULL);
	}
	return ((void *)record->addr);
}

void				*core_allocator(size_t *size)
{
	return ((*size <= MEDIUM_LIMIT) ?
		core_allocator_tiny_medium(size) :
		core_allocator_large(size));
}
