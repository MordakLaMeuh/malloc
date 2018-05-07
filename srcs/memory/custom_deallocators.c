/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom_deallocators.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"
#include "binary_tree/btree_internal_header.h"

void					node_custom_deallocator(void *node)
{
	struct s_node_page	*end_node_page;
	struct s_node		*src_node;

	end_node_page = ctx.node_pages_entry;
	assert(end_node_page->primary_block.nb_node > 0);
	src_node = &end_node_page->node[end_node_page->primary_block.nb_node - 1];
	if (node != (void *)src_node)
		btree_memory_move(node, src_node);
	end_node_page->primary_block.nb_node -= 1;
	if (end_node_page->primary_block.nb_node == 0 &&
			end_node_page->primary_block.next != NULL)
	{
		ctx.node_pages_entry = end_node_page->primary_block.next;
		destroy_pages(end_node_page, NODE_REQ_PAGES * ctx.page_size);
	}
}

void					index_custom_deallocator(struct s_index *index)
{
	struct s_index_page	*end_index_page;
	struct s_index		*src_index;
	struct s_node		*last_node;

	end_index_page = ctx.index_pages_entry;
	assert(end_index_page->primary_block.nb_index > 0);
	src_index =
			&end_index_page->index[end_index_page->primary_block.nb_index - 1];
	if (index != (void *)src_index)
	{
		last_node = get_associated_node_for_index(src_index);
		assert(last_node != NULL);
		ft_aligned_memcpy(index, src_index, sizeof(struct s_index));
		btree_attach_content(last_node, (void *)index, NULL);
	}
	end_index_page->primary_block.nb_index -= 1;
	if (end_index_page->primary_block.nb_index == 0 &&
			end_index_page->primary_block.next != NULL)
	{
		ctx.index_pages_entry = end_index_page->primary_block.next;
		destroy_pages(end_index_page, INDEX_REQ_PAGES * ctx.page_size);
	}
}

/*
** Delete a useless record field. Destroy a page if needed.
*/

void					record_custom_deallocator(struct s_record *record)
{
	struct s_record_page	*end_record_page;
	struct s_record			*src_record;
	struct s_node			*last_node;

	end_record_page = ctx.record_pages_entry;
	assert(end_record_page->primary_block.nb_record > 0);
	src_record =
		&end_record_page->record[end_record_page->primary_block.nb_record - 1];
	if (record != (void *)src_record)
	{
		last_node = get_associated_node_for_record(record);
		assert(last_node != NULL);
		ft_aligned_memcpy(record, src_record, sizeof(struct s_record));
		btree_attach_content(last_node, (void *)record, NULL);
	}
	end_record_page->primary_block.nb_record -= 1;
	if (end_record_page->primary_block.nb_record == 0 &&
			end_record_page->primary_block.next != NULL)
	{
		ctx.record_pages_entry = end_record_page->primary_block.next;
		destroy_pages(end_record_page, RECORD_REQ_PAGES * ctx.page_size);
	}
}
