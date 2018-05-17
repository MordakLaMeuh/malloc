/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_prototypes.next.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctor.h"
#include "main_headers.h"

extern pthread_mutex_t g_mut;

void			*reallocf(void *ptr, size_t size)
{
	void *addr;
	bool memfail;

	pthread_mutex_lock(&g_mut);
	memfail = false;
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ptr == NULL)
	{
		addr = core_allocator(&size);
		if (addr == NULL)
			memfail = true;
	}
	else
		addr = core_realloc(ptr, &size, &memfail);
	if (memfail == true)
		core_deallocator(ptr);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*valloc(size_t size)
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
	size = allign_size(size, LARGE);
	addr = core_allocator_large(&size);
	if (addr == NULL)
	{
		show_alloc_mem();
		errno = ENOMEM;
	}
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			show_alloc_mem(void)
{
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	show_alloc();
	pthread_mutex_unlock(&g_mut);
}
