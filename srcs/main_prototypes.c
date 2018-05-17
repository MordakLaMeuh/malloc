/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_prototypes.c                                  :+:      :+:    :+:   */
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

	pthread_mutex_lock(&g_mut);
	if (size == 0)
	{
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	if (ctx.is_initialized == false)
		constructor_runtime();
	addr = core_allocator(&size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*calloc(size_t count, size_t size)
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
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	ft_aligned_bzero(addr, global_size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			free(void *ptr)
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

void			*realloc(void *ptr, size_t size)
{
	void				*addr;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ptr == NULL)
		addr = core_allocator(&size);
	else
		addr = core_realloc(ptr, &size, NULL);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*reallocarray(void *ptr, size_t nmemb, size_t size)
{
	void				*addr;
	size_t				global_size;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (nmemb > 0 && (SIZE_MAX / nmemb) < size)
	{
		show_alloc_mem();
		errno = ENOMEM;
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	global_size = nmemb * size;
	if (ptr == NULL)
		addr = core_allocator(&global_size);
	else
		addr = core_realloc(ptr, &global_size, NULL);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}
