/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_realloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

static void					*substract_large_page(
	struct s_record *record,
	size_t size)
{
	int old_page_count;
	int new_page_count;
	int offset;

	old_page_count = record->size / ctx.page_size;
	new_page_count = size / ctx.page_size;
	offset = old_page_count - new_page_count;
	if (offset != 0)
		destroy_pages(
			(void *)(record->addr + new_page_count * ctx.page_size),
			offset * ctx.page_size);
	record->size = size;
	return ((void *)record->addr);
}

/*
** Simple, basic, just return the page type.
*/

static enum e_page_type		get_page_type(size_t size)
{
	if (size <= TINY_LIMIT)
		return (TINY);
	else if (size <= MEDIUM_LIMIT)
		return (MEDIUM);
	else
		return (LARGE);
}

/*
** Usefull for realloc. Record is not deleted just changed; The data are copied.
** - The copy quantity is determined by the little size
** - We need a aligned size (multiple by 8) to use ft_aligned_memcpy
*/

static void					*copy_another_place(
	struct s_record *record,
	size_t size,
	enum e_page_type page_type)
{
	void				*addr;
	int					ret;

	if (size <= MEDIUM_LIMIT)
		addr = (void *)assign_index(size, page_type);
	else
		addr = get_new_pages(size);
	if (addr == NULL)
	{
		ft_putstr_fd("Cannot allocate new page\n", STDERR_FILENO);
		return (NULL);
	}
	ft_aligned_memcpy(
		addr,
		(void *)record->addr,
		(record->size < size) ? record->size : size);
	if (record->size <= MEDIUM_LIMIT)
		ret = del_index(record->addr, record->size);
	else
		ret = destroy_pages((void *)record->addr, record->size);
	if (ret < 0)
		ft_putstr_fd("Unexpected error !\n", STDERR_FILENO);
	record->addr = (uint64_t)addr;
	record->size = size;
	return (addr);
}

static int					fill_possible(
	struct s_record *record,
	size_t size,
	enum e_page_type page_type)
{
	struct s_index_page	*index_page;
	struct s_index		*index;
	uint32_t			sector;
	int					i;

	index_page = find_index_page(record->addr, page_type, &i);
	index = &index_page->index[i];
	sector = addr_to_sector(record->addr, index);
	if (try_new_field(
		index,
		sector,
		get_required_sectors(record->size, page_type),
		get_required_sectors(size, page_type)))
	{
		record->size = size;
		return (1);
	}
	return (0);
}

void						*core_realloc(struct s_record *record, size_t size)
{
	enum e_page_type	old_type;
	enum e_page_type	new_type;

	old_type = get_page_type(record->size);
	new_type = get_page_type(size);
	size = allign_size(size, new_type);
	if (old_type != new_type)
		return (copy_another_place(record, size, new_type));
	if (old_type == LARGE)
	{
		if (record->size == size)
		{
			return ((void *)record->addr);
		}
		if (size > record->size)
			return (copy_another_place(record, size, new_type));
		else
			return (substract_large_page(record, size));
	}
	if (fill_possible(record, size, old_type))
		return ((void *)record->addr);
	return (copy_another_place(record, size, new_type));
}

/*
** REALLOC SCHEME
** determine page_type with record_size
** determine new page type with size
** IF Differ
** Allocate a new field		SEQ 1
** Allocate a new record
** copy record_size to new field.
** free old record
** Free old field
** return new record addr.
** IF IDENTICAL
** IF NEW SIZE < OLD_SIZE
** set new field mask			SEQ 2
** modify size of record
** ELSE
** TEST IF FIELD CAN CONTAIN BIGGER FIELD
** IF IT S OKAY.
** DO 							SEQ 2
** ELSE
** DO 							SEQ 1
*/
