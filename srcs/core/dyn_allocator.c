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
#include "ctor.h"

pthread_mutex_t g_mut = PTHREAD_MUTEX_INITIALIZER;

void			*malloc(size_t size)
{
	void		*addr;

	ft_printf("malloc called: size = %lu\n", size);
	if (size == 0)
		return (NULL);
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		main_constructor();
	addr = core_allocator(&size);
	pthread_mutex_unlock(&g_mut);
	ft_printf("addr given = %p\n", addr);
	return (addr);
}

void			*calloc(size_t count, size_t size)
{
	void		*addr;
	size_t		global_size;

	ft_printf("calloc called: size = %lu\n", size);
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
	ft_printf("addr given = %p\n", addr);
	return (addr);
}

void			free(void *ptr)
{
	ft_printf("free called: ptr = %p\n", ptr);
	if (ptr == NULL)
		return ;
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		main_constructor();
	core_deallocator(ptr);
	pthread_mutex_unlock(&g_mut);
}


void			*realloc_cases(void *ptr, size_t size)
{
	struct s_record		*record;

	if (ctx.is_initialized == false)
		main_constructor();
	if (ptr == NULL)
		return (core_allocator(&size));
	if ((record = search_record((uint64_t)ptr)) == NULL)
		return (NULL);
	if (size == 0)
	{
		core_deallocator(ptr);
		return (NULL);
	}
	if (size == record->size)
		return ((void *)record->addr);
	return (core_realloc(record, size));
}

//	ft_printf("realloc called: ptr = %p, size = %lu\n", ptr, size);
//	ft_printf("addr given = %p\n", addr);

void			*realloc(void *ptr, size_t size)
{
	void				*addr;

	pthread_mutex_lock(&g_mut);
	addr = realloc_cases(ptr, size);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			show_alloc_mem(void)
{
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		main_constructor();
	core_debug();
	pthread_mutex_unlock(&g_mut);
}
