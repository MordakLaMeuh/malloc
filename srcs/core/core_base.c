/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

static void			*core_allocator_large(size_t size)
{
	void			*addr;
	struct s_record	*record;

	addr = get_new_pages(size);
	if (addr == NULL)
	{
		ft_putstr_fd("Cannot allocate new pages\n", STDERR_FILENO);
		return (NULL);
	}
	record = get_new_record();
	if (record == NULL)
	{
		ft_putstr_fd("Cannot allocate new record page\n", STDERR_FILENO);
		destroy_pages(addr, size);
		return (NULL);
	}
	record->addr = (uint64_t)addr;
	record->size = size;
	return (addr);
}

static void			*core_allocator_tiny_medium(size_t size)
{
	void			*addr;
	struct s_record	*record;

	addr = (void *)assign_index(size);
	if (addr == NULL)
	{
		ft_putstr_fd("Cannot allocate new index\n", STDERR_FILENO);
		return (NULL);
	}
	record = get_new_record();
	if (record == NULL)
	{
		ft_putstr_fd("Cannot allocate new record page\n", STDERR_FILENO);
		del_index((uint64_t)addr, size);
		return (NULL);
	}
	record->addr = (uint64_t)addr;
	record->size = size;
	return (addr);
}

void				*core_allocator(size_t size)
{
	return ((size <= MEDIUM_LIMIT) ?
		core_allocator_tiny_medium(size) :
		core_allocator_large(size));
}

void				core_deallocator(void *addr)
{
	struct s_record *record;
	int				ret;

	if ((record = search_record((uint64_t)addr)) == NULL)
	{
		ft_putstr_fd("Double free or corruption\n", STDERR_FILENO);
		exit (1);
	}
	if (record->size <= MEDIUM_LIMIT)
		ret = del_index(record->addr, record->size);
	else
		ret = destroy_pages((void *)record->addr, record->size);
	if (ret < 0)
	{
		ft_putstr_fd("Unexpected error at del index", STDERR_FILENO);
		exit (1);
	}
	ret = del_record(record);
	if (ret < 0)
	{
		ft_putstr_fd("Unexpected error at del record", STDERR_FILENO);
		exit (1);
	}
}
