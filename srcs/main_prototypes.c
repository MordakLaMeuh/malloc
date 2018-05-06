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

#include "ctor.h"
#include "main_headers.h"

pthread_mutex_t g_mut = PTHREAD_MUTEX_INITIALIZER;

void			*malloc(size_t size)
{
	void		*addr;

	if (size == 0)
		return (NULL);
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		main_constructor();
	addr = core_allocator(&size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*calloc(size_t count, size_t size)
{
	void		*addr;
	size_t		global_size;

	global_size = count * size;
	if (global_size == 0)
		return (NULL);
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		main_constructor();
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

void			free(void *ptr)
{
	if (ptr == NULL)
		return ;
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		main_constructor();
	core_deallocator(ptr);
	pthread_mutex_unlock(&g_mut);
}

void			*realloc(void *ptr, size_t size)
{
	void				*addr;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		main_constructor();
	addr = core_realloc(ptr, size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			show_alloc_mem(void)
{
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		main_constructor();
	pthread_mutex_unlock(&g_mut);
}
