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

static void			*core_allocator_large(size_t *size)
{
	void			*addr;
	struct s_record	*record;

	*size = allign_size(*size, LARGE);
	addr = get_new_pages(*size);
	if (addr == NULL)
	{
		ft_putstr_fd("Cannot allocate new pages\n", STDERR_FILENO);
		return (NULL);
	}
	record = get_new_record();
	if (record == NULL)
	{
		ft_putstr_fd("Cannot allocate new record page\n", STDERR_FILENO);
		destroy_pages(addr, *size);
		return (NULL);
	}
	record->addr = (uint64_t)addr;
	record->size = *size;
	return (addr);
}

static void			*core_allocator_tiny_medium(size_t *size)
{
	void				*addr;
	struct s_record		*record;
	enum e_page_type	page_type;

	page_type = (*size <= TINY_LIMIT) ? TINY : MEDIUM;
	*size = allign_size(*size, page_type);
	addr = (void *)assign_index(*size, page_type);
	if (addr == NULL)
	{
		ft_putstr_fd("Cannot allocate new index\n", STDERR_FILENO);
		return (NULL);
	}
	record = get_new_record();
	if (record == NULL)
	{
		ft_putstr_fd("Cannot allocate new record page\n", STDERR_FILENO);
		del_index((uint64_t)addr, *size);
		return (NULL);
	}
	record->addr = (uint64_t)addr;
	record->size = *size;
	return (addr);
}

void				*core_allocator(size_t *size)
{
	return ((*size <= MEDIUM_LIMIT) ?
		core_allocator_tiny_medium(size) :
		core_allocator_large(size));
}
