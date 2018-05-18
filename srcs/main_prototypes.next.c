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

#include "main_headers.h"

extern pthread_mutex_t g_mut;

void			*reallocf(void *ptr, size_t size)
{
	void *addr;
	bool memfail;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ctx.tracer_file_descriptor != -1)
		begin_trace(REALLOCF, ptr, size, 0);
	memfail = false;
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
	if (ctx.tracer_file_descriptor != -1)
		bend_trace(memfail == false ? SUCCESS : FAIL);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*valloc(size_t size)
{
	void		*addr;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ctx.tracer_file_descriptor != -1)
		begin_trace(VALLOC, NULL, size, 0);
	if (size == 0)
	{
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(NO_OP);
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	size = allign_size(size, LARGE);
	if ((addr = core_allocator_large(&size)) == NULL)
	{
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(FAIL);
		errno = ENOMEM;
	}
	else if (ctx.tracer_file_descriptor != -1)
		bend_trace(SUCCESS);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			show_alloc_mem(void)
{
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	show_alloc(false);
	pthread_mutex_unlock(&g_mut);
}

void			show_alloc_mem_ex(void)
{
	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	show_alloc(true);
	pthread_mutex_unlock(&g_mut);
}
