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

void			*ft_malloc(size_t size)
{
	void		*addr;

	ft_printf("--- MALLOC ---\n");
	pthread_mutex_lock(&g_mut);
	if (size == 0)
	{
		errno = ENOMEM;
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	if (ctx.is_initialized == false)
		constructor_runtime();
	addr = core_allocator(&size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*ft_calloc(size_t count, size_t size)
{
	void		*addr;
	size_t		global_size;

	pthread_mutex_lock(&g_mut);
	global_size = count * size;
	if (global_size == 0)
	{
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	if (ctx.is_initialized == false)
		constructor_runtime();
	addr = core_allocator(&global_size);
	if (addr == NULL)
	{
		errno = ENOMEM;
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	ft_aligned_bzero(addr, global_size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			ft_free(void *ptr)
{
	pthread_mutex_lock(&g_mut);
	if (ptr == NULL)
	{
		pthread_mutex_unlock(&g_mut);
		return ;
	}
	if (ctx.is_initialized == false)
		constructor_runtime();
	core_deallocator(ptr);
	pthread_mutex_unlock(&g_mut);
}

void			*ft_realloc(void *ptr, size_t size)
{
	void				*addr;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	addr = core_realloc(ptr, size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			ft_show_alloc_mem(void)
{
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	show_alloc();
	pthread_mutex_unlock(&g_mut);
}
