/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_custom_deallocator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static void				*find_free_node(
		struct s_node *node,
		enum e_node_type type)
{
	struct s_node *sub_node;

	sub_node = (struct s_node *)btree_get_node_by_content(
			(type == RECORD_FREE_TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&node->m.size,
			&cmp_size_to_node_size);
	if (sub_node == NULL)
		return (NULL);
	return (&sub_node->ptr_a);
}

static struct s_node	**find_root(struct s_node *node)
{
	void *ptr;

	ptr = NULL;
	if (node->mask.s.node_type == INDEX)
		return (&ctx.index_pages_tree);
	else if (node->mask.s.node_type == RECORD_ALLOCATED_LARGE)
		return (&ctx.big_page_record_tree);
	else if (node->mask.s.node_type == PARENT_RECORD_FREE_TINY)
		return (&ctx.global_tiny_space_tree);
	else if (node->mask.s.node_type == PARENT_RECORD_FREE_MEDIUM)
		return (&ctx.global_medium_space_tree);
	else if (node->mask.s.node_type == RECORD_ALLOCATED_TINY)
		ptr = find_index_node(node->ptr_a);
	else if (node->mask.s.node_type == RECORD_ALLOCATED_MEDIUM)
		ptr = find_index_node(node->ptr_a);
	else if (node->mask.s.node_type == RECORD_FREE_TINY)
		ptr = find_free_node(node, RECORD_FREE_TINY);
	else if (node->mask.s.node_type == RECORD_FREE_MEDIUM)
		ptr = find_free_node(node, RECORD_FREE_MEDIUM);
	return (ptr);
}

static void				destroy_node_page(struct s_node_page *end_node_page)
{
	if (end_node_page->primary_block.next == NULL)
	{
		if (ctx.node_cache)
		{
			destroy_pages(ctx.node_cache, NODE_REQ_PAGES * ctx.page_size);
			ctx.node_cache = NULL;
		}
	}
	else
	{
		if (ctx.node_cache)
			destroy_pages(ctx.node_cache, NODE_REQ_PAGES * ctx.page_size);
		ctx.node_cache = end_node_page;
		ctx.node_pages_entry = end_node_page->primary_block.next;
	}
}

void					node_custom_deallocator(void *node)
{
	struct s_node_page	*end_node_page;
	struct s_node		*src_node;
	uint64_t			*ptr;

	end_node_page = ctx.node_pages_entry;
	src_node = &end_node_page->node[end_node_page->primary_block.nb_node - 1];
	if (node != (void *)src_node)
	{
		if (((struct s_node *)src_node)->parent == NULL)
		{
			ptr = (uint64_t *)find_root(src_node);
			*ptr = (uint64_t)node;
		}
		btree_memory_move(node, src_node);
	}
	ctx.size_owned_by_nodes -= sizeof(struct s_node);
	end_node_page->primary_block.nb_node -= 1;
	if (end_node_page->primary_block.nb_node == 0)
		destroy_node_page(end_node_page);
}
