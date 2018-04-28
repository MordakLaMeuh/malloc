/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

pthread_mutex_t g_mut = PTHREAD_MUTEX_INITIALIZER;

void			*ft_malloc(size_t size)
{
	void		*addr;

	if (size == 0)
		return (NULL);
	pthread_mutex_lock(&g_mut);
	addr = core_allocator(&size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*ft_calloc(size_t count, size_t size)
{
	void		*addr;
	size_t		global_size;

	global_size = count * size;
	if (global_size == 0)
		return (NULL);
	pthread_mutex_lock(&g_mut);
	addr = core_allocator(&global_size);
	if (addr == NULL)
	{
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	ft_aligned_bzero(addr, global_size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			ft_free(void *ptr)
{
	if (ptr == NULL)
		return ;
	pthread_mutex_lock(&g_mut);
	core_deallocator(ptr);
	pthread_mutex_unlock(&g_mut);
}

void			*ft_realloc_2(void *ptr, size_t size)
{
	struct s_record		*record;
	void				*addr;

	if (ptr == NULL)
	{
		addr = core_allocator(&size);
		return (addr);
	}
	if ((record = search_record((uint64_t)ptr)) == NULL)
	{
		ft_putstr_fd("Double free or corruption\n", STDERR_FILENO);
		exit(1);
	}
	if (size == 0)
	{
		core_deallocator(ptr);
		return (NULL);
	}
	if (size == record->size)
		return ((void *)record->addr);
	addr = core_realloc(record, size);
	return (addr);
}

void			*ft_realloc(void *ptr, size_t size)
{
	void				*addr;

	pthread_mutex_lock(&g_mut);
	addr = ft_realloc_2(ptr, size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			ft_debug_allocator(void)
{
	pthread_mutex_lock(&g_mut);
	core_debug();
	pthread_mutex_unlock(&g_mut);
}
