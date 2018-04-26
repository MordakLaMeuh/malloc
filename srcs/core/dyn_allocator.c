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

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void				*ft_malloc(size_t size)
{
	void			*addr;

	pthread_mutex_lock(&mut);
	addr = core_allocator(size);
#ifdef MAIN_DEBUG
	printf("malloc associated %p. size:%lu\n", addr, size);
#endif
	pthread_mutex_unlock(&mut);
	return (addr);
}

void				*ft_calloc(size_t count, size_t size)
{
	void			*addr;
	size_t			global_size;

	pthread_mutex_lock(&mut);
	global_size = count * size;
	addr = core_allocator(size);
	if (addr == NULL)
	{
		pthread_mutex_unlock(&mut);
		return (NULL);
	}
	ft_aligned_bzero(addr, global_size);
#ifdef MAIN_DEBUG
	printf("calloc associated %p. size:%lu\n", addr, size);
#endif
	pthread_mutex_unlock(&mut);
	return (NULL);
}

void				ft_free(void *ptr)
{
	struct s_record	*record;
	int				ret;

#ifdef MAIN_DEBUG
	printf("freeing addr: %p\n", ptr);
#endif
	if (ptr == NULL)
		return ;
	pthread_mutex_lock(&mut);
	if ((record = search_record((uint64_t)ptr)) == NULL)
	{
		ft_putstr_fd("Double free or corruption\n", STDERR_FILENO);
		pthread_mutex_unlock(&mut);
		exit (1);
	}
	if (record->size <= MEDIUM_LIMIT)
		ret = del_index(record->addr, record->size);
	else
		ret = destroy_pages((void *)record->addr, record->size);
	if (ret < 0 || del_record(record) < 0)
	{
		ft_putstr_fd("Unexpected error at free", STDERR_FILENO);
		pthread_mutex_unlock(&mut);
		exit (1);
	}
	pthread_mutex_unlock(&mut);
}

void			*ft_realloc(void *ptr, size_t size)
{
	pthread_mutex_lock(&mut);
	// XXX TODO
	(void)ptr;
	(void)size;
	pthread_mutex_unlock(&mut);
	return (NULL);
}

void			ft_debug_allocator(void)
{
	pthread_mutex_lock(&mut);
	pthread_mutex_unlock(&mut);
}
