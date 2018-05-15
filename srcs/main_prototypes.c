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

	ft_dprintf(B_DEBUG, "{yellow}- - - - - MALLOC - - - - -{eoc}\n");
	pthread_mutex_lock(&g_mut);
	if (size == 0)
	{
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	if (ctx.is_initialized == false)
		constructor_runtime();
	addr = core_allocator(&size);
	if (addr == NULL)
	{
		ft_printf("%s ENOMEM: %lu\n", __func__, size);
		ft_show_alloc_mem();
		errno = ENOMEM;
	}
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
		ft_printf("%s ENOMEM: %lu x %lu\n", __func__, count, size);
		ft_show_alloc_mem();
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
	ft_dprintf(B_DEBUG, "{yellow}- - - - - FREE - - - - -{eoc}\n");
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

	ft_dprintf(B_DEBUG, "{yellow}- - - - - REALLOC - - - - -{eoc}\n");
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ptr == NULL)
		addr = core_allocator(&size);
	else
		addr = core_realloc(ptr, &size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*ft_reallocarray(void *ptr, size_t nmemb, size_t size)
{
	void				*addr;
	size_t				global_size;

	ft_dprintf(B_DEBUG, "{yellow}- - - - - REALLOCARRAY - - - - -{eoc}\n");
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (nmemb > 0 && (SIZE_MAX / nmemb) < size)
	{
		ft_printf("%s ENOMEM: (overflow) %lu x %lu\n", __func__, nmemb, size);
		ft_show_alloc_mem();
		errno = ENOMEM;
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	global_size = nmemb * size;
	if (ptr == NULL)
		addr = core_allocator(&global_size);
	else
		addr = core_realloc(ptr, &global_size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}
