/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctor.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

extern pthread_mutex_t g_mut;

#ifndef __APPLE__
# define GETPAGESIZE() (size_t)sysconf(_SC_PAGESIZE)
#else
# define GETPAGESIZE() getpagesize()
#endif

static void	fill_preallocated_chunk_next(char *base_addr)
{
	ctx.node_density = (NODE_REQ_PAGES * ctx.page_size -
			sizeof(struct s_primary_node)) / btree_get_node_size();
	ctx.global_tiny_space_tree = btree_new();
	create_index(base_addr, TINY);
	insert_free_record(base_addr, TINY_RANGE, TINY, NULL);
	base_addr += TINY_RANGE;
	ctx.global_medium_space_tree = btree_new();
	create_index(base_addr, MEDIUM);
	insert_free_record(base_addr, MEDIUM_RANGE, MEDIUM, NULL);
}

static void	fill_preallocated_chunk(char *base_addr)
{
	ctx.node_pages_entry = (struct s_node_page *)base_addr;
	ctx.node_pages_entry->primary_block.nb_node = 0;
	ctx.node_pages_entry->primary_block.next = NULL;
	ctx.node_cache = NULL;
	base_addr += NODE_REQ_PAGES * ctx.page_size;
	ctx.big_page_record_tree = btree_new();
	ctx.index_pages_tree = btree_new();
	fill_preallocated_chunk_next(base_addr);
}

void		constructor_runtime(void)
{
	int		ret;
	size_t	preallocated_size;
	void	*base_addr;

	ctx.page_size = GETPAGESIZE();
	if ((ret = getrlimit(RLIMIT_DATA, &ctx.mem_limit)) < 0)
	{
		ft_eprintf("dyn_allocator cannot get RLIMIT_DATA\n");
		exit(1);
	}
	preallocated_size = NODE_REQ_PAGES * ctx.page_size;
	preallocated_size += TINY_RANGE;
	preallocated_size += MEDIUM_RANGE;
	if ((base_addr = get_new_pages(preallocated_size)) == NULL)
	{
		ft_eprintf("failed to allocate base preallocated memory\n");
		exit(1);
	}
	fill_preallocated_chunk(base_addr);
	ctx.is_initialized = true;
}

void		main_constructor(void)
{
	pthread_mutex_lock(&g_mut);
	constructor_runtime();
	pthread_mutex_unlock(&g_mut);
}

void		main_destructor(void)
{
	pthread_mutex_lock(&g_mut);
	pthread_mutex_unlock(&g_mut);
}
