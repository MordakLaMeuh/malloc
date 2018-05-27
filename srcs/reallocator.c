/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocator.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static void					*copy_another_place(
		struct s_node *record,
		size_t *size)
{
	void		*dst_addr;
	void		*src_addr;
	size_t		src_size;

	src_addr = record->ptr_a;
	src_size = record->m.size;
	dst_addr = core_allocator(size);
	if (dst_addr == NULL)
		return (NULL);
	ft_aligned_memcpy(dst_addr, src_addr, src_size < *size ? src_size : *size);
	core_deallocator(src_addr);
	return (dst_addr);
}

void						view_next_record(
		struct s_node *record,
		struct s_node *index,
		enum e_page_type type,
		struct s_couple *n_free_record)
{
	struct s_node *next_allocated_record;

	next_allocated_record = btree_get_next_neighbours_node(record);
	if (next_allocated_record != NULL)
	{
		n_free_record->addr = (void *)((uint64_t)record->ptr_a
				+ (uint64_t)record->m.size);
		n_free_record->len = (uint64_t)next_allocated_record->ptr_a
				- (uint64_t)record->ptr_a - record->m.size;
	}
	else if ((uint64_t)record->ptr_a + record->m.size != index->m.size
			+ ((type == TINY) ? TINY_RANGE : MEDIUM_RANGE))
	{
		n_free_record->len = (uint64_t)index->m.size + ((type == TINY) ?
				TINY_RANGE : MEDIUM_RANGE) - ((uint64_t)record->ptr_a
						+ record->m.size);
		n_free_record->addr = (void *)((uint64_t)record->ptr_a +
				(uint64_t)record->m.size);
	}
}

/*
** determine if free size
** search next allocated record
** search free record at (uint8_t)record->ptr_a + record->m.size
** copy to n_free_record
** modify record size
** destroy free record
** insert a new free record (n_free_record.addr, n_free_record.len)
*/

static void					*fill_possible(
	struct s_node *record,
	struct s_node *index,
	size_t size,
	enum e_page_type type)
{
	struct s_couple		n_free_record;
	struct s_node		*next_free_record;
	struct s_node		*free_record_parent;
	void				*addr;

	n_free_record.len = 0;
	view_next_record(record, index, type, &n_free_record);
	next_free_record = NULL;
	if (n_free_record.len > 0)
		next_free_record = get_free_record(n_free_record.addr,
				n_free_record.len, &free_record_parent, type);
	n_free_record.len = (next_free_record) ? next_free_record->m.size : 0;
	if ((n_free_record.len + record->m.size) < size)
		return (NULL);
	addr = record->ptr_a;
	ctx.size_owned_by_data -= record->m.size;
	n_free_record.len = (n_free_record.len + record->m.size) - size;
	n_free_record.addr = (uint8_t *)record->ptr_a + size;
	record->m.size = size;
	ctx.size_owned_by_data += record->m.size;
	if (next_free_record)
		delete_free_record(next_free_record, free_record_parent, type);
	if (n_free_record.len != 0)
		insert_free_record(n_free_record.addr, n_free_record.len, type, NULL);
	return (addr);
}

static void					*reallocator(
		struct s_node *record,
		struct s_node *index,
		size_t *size)
{
	enum e_page_type	old_type;
	enum e_page_type	new_type;
	void				*addr;

	old_type = get_page_type(record->m.size);
	new_type = get_page_type(*size);
	*size = allign_size(*size, new_type);
	if (old_type != new_type)
		return (copy_another_place(record, size));
	if (record->m.size == *size)
		return (record->ptr_a);
	if (old_type == LARGE)
	{
		if (*size > record->m.size)
			return (copy_another_place(record, size));
		else
			return (substract_large_page(record, *size));
	}
	addr = fill_possible(record, index, *size, old_type);
	if (addr)
		return (addr);
	return (copy_another_place(record, size));
}

/*
** Memfail pointer is set to TRUE when a memory problem occur.
*/

void						*core_realloc(
		void *ptr,
		size_t *size,
		bool *memfail)
{
	struct s_node		*record;
	struct s_node		*index;
	void				*addr;

	index = NULL;
	*memfail = false;
	if ((record = btree_get_node_by_content(ctx.big_page_record_tree,
			ptr, &cmp_addr_to_node_addr)) == NULL)
		index = (struct s_node *)btree_get_node_by_content(
			ctx.index_pages_tree, ptr, cmp_addr_to_node_m_addr_range);
	if (record == NULL && index != NULL)
	{
		record = btree_get_node_by_content(index->ptr_a, ptr,
			&cmp_addr_to_node_addr);
	}
	if (record == NULL)
	{
		if (record == NULL || core_deallocator(ptr) < 0)
			*memfail = true;
		return (NULL);
	}
	if ((addr = reallocator(record, index, size)) == NULL)
		*memfail = true;
	return (addr);
}
