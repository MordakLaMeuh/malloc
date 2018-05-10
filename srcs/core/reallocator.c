/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static void					*substract_large_page(
		struct s_index *index,
		struct s_node *node,
		size_t size)
{
	(void)index;
	(void)node;
	(void)size;
	return (NULL);
}

/*
** Usefull for realloc. Record is not deleted just changed; The data are copied.
** - The copy quantity is determined by the little size
** - We need a aligned size (multiple by 8) to use ft_aligned_memcpy
*/

static void					*copy_another_place(
		struct s_index *index,
		struct s_node *node,
		size_t size,
		enum e_page_type page_type)
{
	(void)index;
	(void)node;
	(void)size;
	(void)page_type;
	return (NULL);
}

static int					fill_possible(
	struct s_index *index,
	struct s_node *node,
	size_t size,
	enum e_page_type page_type)
{
	(void)index;
	(void)node;
	(void)size;
	(void)page_type;
	return (0);
}

static void					*reallocator(
		struct s_node *node,
		struct s_index *index,
		size_t size)
{
	struct s_record		*record;
	enum e_page_type	old_type;
	enum e_page_type	new_type;

	record = btree_get_node_content(node);
	old_type = get_page_type(record->size);
	new_type = get_page_type(size);
	size = allign_size(size, new_type);
	if (old_type != new_type)
		return (copy_another_place(index, node, size, new_type));
	if (old_type == LARGE)
	{
		if (record->size == size)
			return ((void *)record->addr);
		if (size > record->size)
			return (copy_another_place(index, node, size, new_type));
		else
			return (substract_large_page(index, node, size));
	}
	if (fill_possible(index, node, size, old_type))
		return ((void *)record->addr);
	return (copy_another_place(index, node, size, new_type));
}

void						*core_realloc(void *ptr, size_t size)
{
	struct s_node		*node;
	struct s_record		*record;
	struct s_index		*index;
	void				*out;

	if ((node = search_record_node((uint64_t)ptr, &index)) == NULL)
		return (NULL);
	record = (struct s_record *)btree_get_node_content(node);
	if (record == NULL)
		return (NULL);
	if (size == 0)
	{
		core_deallocator(ptr);
		return (NULL);
	}
	if (size == record->size)
		return ((void *)record->addr);
	out = reallocator(node, index, size);
	if (out == NULL)
		errno = ENOMEM;
	return (out);
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
