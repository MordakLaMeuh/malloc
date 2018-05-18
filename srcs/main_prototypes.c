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

#include "main_headers.h"

pthread_mutex_t g_mut = PTHREAD_MUTEX_INITIALIZER;

void			*malloc(size_t size)
{
	void		*addr;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ctx.tracer_file_descriptor != -1)
		begin_trace(MALLOC, NULL, size, 0);
	if (size == 0)
	{
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(NO_OP);
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	addr = core_allocator(&size);
	if (ctx.tracer_file_descriptor != -1)
		bend_trace(addr != NULL ? SUCCESS : FAIL);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*calloc(size_t count, size_t size)
{
	void		*addr;
	size_t		global_size;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ctx.tracer_file_descriptor != -1)
		begin_trace(CALLOC, NULL, count, size);
	if ((global_size = count * size) == 0)
	{
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(NO_OP);
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	if ((addr = core_allocator(&global_size)) == NULL)
	{
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(FAIL);
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	ft_aligned_bzero(addr, global_size);
	if (ctx.tracer_file_descriptor != -1)
		bend_trace(SUCCESS);
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			free(void *ptr)
{
	int ret;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ctx.tracer_file_descriptor != -1)
		begin_trace(FREE, ptr, 0, 0);
	if (ptr == NULL)
	{
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(NO_OP);
		pthread_mutex_unlock(&g_mut);
		return ;
	}
	ret = core_deallocator(ptr);
	if (ctx.tracer_file_descriptor != -1)
		bend_trace(ret < 0 ? FAIL : SUCCESS);
	pthread_mutex_unlock(&g_mut);
}

void			*realloc(void *ptr, size_t size)
{
	void				*addr;
	bool				memfail;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ctx.tracer_file_descriptor != -1)
		begin_trace(REALLOC, ptr, size, 0);
	if (ptr == NULL)
	{
		addr = core_allocator(&size);
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(addr != NULL ? SUCCESS : FAIL);
	}
	else
	{
		addr = core_realloc(ptr, &size, &memfail);
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(memfail == false ? SUCCESS : FAIL);
	}
	pthread_mutex_unlock(&g_mut);
	return (addr);
}

void			*reallocarray(void *ptr, size_t nmemb, size_t size)
{
	void				*addr;
	size_t				global_size;
	bool				memfail;

	pthread_mutex_lock(&g_mut);
	if (ctx.is_initialized == false)
		constructor_runtime();
	if (ctx.tracer_file_descriptor != -1)
		begin_trace(REALLOCARRAY, ptr, nmemb, size);
	if (nmemb > 0 && (SIZE_MAX / nmemb) < size)
	{
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(FAIL);
		errno = ENOMEM;
		pthread_mutex_unlock(&g_mut);
		return (NULL);
	}
	global_size = nmemb * size;
	if (ptr == NULL)
	{
		addr = core_allocator(&global_size);
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(addr != NULL ? SUCCESS : FAIL);
	}
	else
	{
		addr = core_realloc(ptr, &global_size, &memfail);
		if (ctx.tracer_file_descriptor != -1)
			bend_trace(memfail == false ? SUCCESS : FAIL);
	}
	pthread_mutex_unlock(&g_mut);
	return (addr);
}
