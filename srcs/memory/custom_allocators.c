/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_allocators.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

/*
** XXX Be careful with the size formula.
*/

void				*node_custom_allocator(size_t size)
{
	struct s_node_page		*node_page;
	struct s_node_page		*new_node_page;
	void					*addr;

	node_page = ctx.node_pages_entry;
	if (node_page->primary_block.nb_node == ctx.node_density)
	{
		new_node_page = (struct s_node_page *)
				get_new_pages(NODE_REQ_PAGES * ctx.page_size);
		if (new_node_page == NULL)
			return (NULL);
		new_node_page->primary_block.nb_node = 0;
		new_node_page->primary_block.next = node_page;
		node_page->primary_block.next = NULL;
		ctx.node_pages_entry = new_node_page;
		node_page = new_node_page;
	}
	addr = (char *)&node_page->node + (size * node_page->primary_block.nb_node);
	node_page->primary_block.nb_node += 1;
	ft_printf("new node created at: %p\n", addr);
	return (addr);
}

/*
** Request a new index field in index_page, create a new index page
** if necessary.
*/

struct s_index		*index_custom_allocator(void)
{
	struct s_index_page		*index_page;
	struct s_index_page		*new_index_page;
	void					*addr;

	index_page = ctx.index_pages_entry;
	if (index_page->primary_block.nb_index == ctx.index_density)
	{
		new_index_page = (struct s_index_page *)
				get_new_pages(INDEX_REQ_PAGES * ctx.page_size);
		if (new_index_page == NULL)
			return (NULL);
		new_index_page->primary_block.nb_index = 0;
		new_index_page->primary_block.next = index_page;
		index_page->primary_block.next = NULL;
		ctx.index_pages_entry = new_index_page;
		index_page = new_index_page;
	}
	addr = &index_page->index[index_page->primary_block.nb_index];
	index_page->primary_block.nb_index += 1;
	return ((struct s_index *)addr);
}

/*
** Return a new record field. Create an new record chunk if needed.
*/

struct s_record		*record_custom_allocator(void)
{
	struct s_record_page	*record_page;
	struct s_record_page	*new_record_page;
	void					*addr;

	record_page = ctx.record_pages_entry;
	if (record_page->primary_block.nb_record == ctx.record_density)
	{
		new_record_page = (struct s_record_page *)
				get_new_pages(RECORD_REQ_PAGES * ctx.page_size);
		if (new_record_page == NULL)
			return (NULL);
		new_record_page->primary_block.nb_record = 0;
		new_record_page->primary_block.next = record_page;
		record_page->primary_block.next = NULL;
		ctx.record_pages_entry = new_record_page;
		record_page = new_record_page;
	}
	addr = &record_page->record[record_page->primary_block.nb_record];
	record_page->primary_block.nb_record += 1;
	return ((struct s_record *)addr);
}
