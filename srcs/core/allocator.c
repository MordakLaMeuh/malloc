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


static uint64_t		find_index_and_set(
		size_t size,
		enum e_page_type page_type,
		struct s_index **index
)
{
	struct s_node	*free_record_tree_node;
	struct s_node	*free_record_node;
	struct s_record	*record;
	uint64_t		addr;
	bool			alone_node;

	free_record_tree_node = get_free_record_node(size, page_type);
//	assert(free_record_tree_node != NULL);
	if (free_record_tree_node == NULL)
	{
		// ALLOCATE PAGES			// step 1 -
		// CREATE A PAGE INDEX		// step 1 -
		// CREATE A FREE NODE		// step 1 -
		// RETURN POINTER TO THIS FREE NODE (begin tree) // step 1
		// Associate to free_record_tree_node
		ft_printf("reserving new pages\n");
//		exit (0);
		*index = reserve_new_chunk(page_type, &free_record_tree_node);
	}
	// Take the first suitable space we found
	assert(free_record_tree_node != NULL);
	free_record_node = (struct s_node *)
			btree_get_node_content(free_record_tree_node);
	assert(free_record_node != NULL);

	record = btree_get_node_content(free_record_node);
	addr = record->addr;

	// find in pages (tiny or medium) the corresponding index
	//address = record->addr & (uint64_t)~0xFFF;
	if (page_type == TINY)
		*index = btree_search_content(ctx.tiny_index_pages_tree,
				&record->addr,
				&cmp_addr_to_tiny_index);

	else
		*index = btree_search_content(ctx.medium_index_pages_tree,
				&record->addr,
				&cmp_addr_to_medium_index);

	// Test if alone				// step 2 - method founded !
	alone_node = (btree_is_last_node(free_record_node)) ? true : false;
	ft_printf("ALONE = %i\n", alone_node);

	// Destroy the free node we found
	btree_delete_rnb_node(&free_record_node, free_record_node,
			&node_custom_deallocator);
	// Deletion problem; if alone
	if (alone_node)
	{
		alone_node = (btree_is_last_node(ctx.global_tiny_space_tree)) ? true : false;
		ft_printf("ALONE = %i\n", alone_node);
		if (page_type == TINY)
			btree_delete_rnb_node(&ctx.global_tiny_space_tree,
					free_record_tree_node, &node_custom_deallocator);
		else
			btree_delete_rnb_node(&ctx.global_medium_space_tree,
					free_record_tree_node, &node_custom_deallocator);
		ft_printf("final addr = %p\n", ctx.global_tiny_space_tree);
	}

	if (record->size > size)
	{
		// Insert a new free node after take the space
		record->size -= size;
		record->addr += size;
		ft_printf("new size = %lu, addr = %p\n", record->size, record->addr);
		add_free_record(record, page_type);
	}
	else
	{
		ft_printf("deallocate record\n");
		record_custom_deallocator(record);
	}
	return (addr);
}

static int			cmp_record_to_record(void *record1, void *record2)
{
	if (((struct s_record *)record1)->addr < ((struct s_record *)record2)->addr)
		return (-1);
	if (((struct s_record *)record1)->addr > ((struct s_record *)record2)->addr)
		return (1);
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
			record, &cmp_record_to_record, &node_custom_allocator);
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
	ft_printf("New allocated insertion:\n");
	if (btree_insert_rnb_node_by_content(&index->allocation_tree,
			record, &cmp_record_to_record, &node_custom_allocator) == NULL)
	{
		ft_printf("Allocation failed !\n");
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
