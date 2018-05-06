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

//	ft_printf("malloc called: size = %lu\n", size);
	if (size == 0)
		return (NULL);
	pthread_mutex_lock(&g_mut);
	if (ctx.page_size == 0)
		main_constructor();
	addr = core_allocator(&size);
	pthread_mutex_unlock(&g_mut);
//	ft_printf("addr given = %p\n", addr);
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
	if (ctx.page_size == 0)
		main_constructor();
	addr = core_allocator(&global_size);
	if (addr == NULL)
	{
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	ft_aligned_bzero(addr, global_size);
	pthread_mutex_unlock(&g_mut);
//	ft_printf("addr given = %p\n", addr);
	return (addr);
}

void			free(void *ptr)
{
//	ft_printf("free called: ptr = %p\n", ptr);
	if (ptr == NULL)
		return ;
	pthread_mutex_lock(&g_mut);
	if (ctx.page_size == 0)
		main_constructor();
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
//		ft_putstr_fd("Double free or corruption\n", STDERR_FILENO);
		return NULL;
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

void			*realloc(void *ptr, size_t size)
{
	void				*addr;

//	ft_printf("realloc called: ptr = %p, size = %lu\n", ptr, size);
	pthread_mutex_lock(&g_mut);
	if (ctx.page_size == 0)
		main_constructor();
	addr = ft_realloc_2(ptr, size);
	pthread_mutex_unlock(&g_mut);
//	ft_printf("addr given = %p\n", addr);
	return (addr);
}

void			ft_show_alloc_mem(void)
{
	pthread_mutex_lock(&g_mut);
	core_debug();
	pthread_mutex_unlock(&g_mut);
}
