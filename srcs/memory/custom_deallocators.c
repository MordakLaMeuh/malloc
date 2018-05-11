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

static int		cmp_sub_size_to_recursive_record(void *size, void *record_node)
{
	struct s_record *record;
	uint64_t *length;

	length = (uint64_t *)size;
	record = btree_get_node_content(record_node);
	if (*length < record->size)
		return (-1);
	if (*length > record->size)
		return (1);
	return (0);
}

int					find_free_node(struct s_node *node)
{
	struct s_record *record;
	struct s_node *sub_node_content;
	enum e_page_type type;

	record = btree_get_node_content(node);

	type = record->size <= TINY_RANGE ? TINY: MEDIUM;
	//ft_printf("type = %s, size = %lu\n", (type == TINY) ? "TINY" : "MEDIUM", record->size);

	sub_node_content = (struct s_node *)btree_search_content(
			(type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&record->size,
			&cmp_sub_size_to_recursive_record);

	//ft_printf("sub_node_tree = %p\n", sub_node_content);
	if (sub_node_content == node)
		return (1);
	return (0);
}

static int			cmp_record_addr_to_tiny_index(void *record, void *index)
{
	if (((struct s_record *)record)->addr < ((struct s_index *)index)->page_addr)
		return (-1);
	if (((struct s_record *)record)->addr >=
			(((struct s_index *)index)->page_addr + TINY_RANGE))
		return (1);
	return (0);
}

static int			cmp_record_addr_to_medium_index(void *record, void *index)
{
	if (((struct s_record *)record)->addr < ((struct s_index *)index)->page_addr)
		return (-1);
	if (((struct s_record *)record)->addr >=
			(((struct s_index *)index)->page_addr + MEDIUM_RANGE))
		return (1);
	return (0);
}

int						find_allocated_node(struct s_node *node)
{
	struct s_record *record;
	struct s_index *index;
	enum e_page_type type;

	record = btree_get_node_content(node);

	type = record->size <= TINY_RANGE ? TINY: MEDIUM;
	type = get_page_type(record->size);
	if (type == LARGE)
		return (0);
	index = (struct s_index *)btree_search_content(
			(type == TINY) ?
			ctx.tiny_index_pages_tree : ctx.medium_index_pages_tree,
			record,
			(type == TINY) ? &cmp_record_addr_to_tiny_index : &cmp_record_addr_to_medium_index);
//	if (index)
//		ft_printf("index founded ! alloc_tree = %p, node = %p, record addr = %p, size = %lu, page = %p, type : %s\n", index->allocation_tree, node, record->addr, record->size, (void *)index->page_addr, (type == TINY)  ? "TINY" : "MEDIUM");
	if (index && index->allocation_tree == node)
			return (1);
	return (0);
}

void					find_root(void *node)
{
	ft_printf("max = %i\n", ctx.node_pages_entry->primary_block.nb_node);

	for (int i = 0; i < ctx.node_pages_entry->primary_block.nb_node; i++)
	{
	node = (uint8_t *)(&ctx.node_pages_entry->node) +
			(i) * btree_get_node_size();

	ft_printf("%.2i -> ", i);
	if (node == ctx.tiny_index_pages_tree)
		ft_printf("tiny_index_pages_tree\n");
	else if (node == ctx.medium_index_pages_tree)
		ft_printf("medium_index_pages_tree\n");
	else if (node == ctx.big_page_record_tree)
		ft_printf("big_page_record_tree\n");
	else if (node == ctx.global_tiny_space_tree)
		ft_printf("global_tiny_space_tree\n");
	else if (node == ctx.global_medium_space_tree)
		ft_printf("global_medium_space_tree\n");
	else if (find_allocated_node((struct s_node *)node))
		ft_printf("Allocated node\n");
	else if (find_free_node((struct s_node *)node))
		ft_printf("Free node\n");
	else
		ft_printf("root not founded !\n");
	}
}

void				*find_serious_free_node(struct s_node *node)
{
	struct s_record *record;
	struct s_node *sub_node_content;
	enum e_page_type type;

	record = btree_get_node_content(node);
	ft_printf("Warning: find_serious_free_node root founded !\n");

	type = record->size <= TINY_RANGE ? TINY: MEDIUM;

	sub_node_content = (struct s_node *)btree_search_content(
			(type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&record->size,
			&cmp_sub_size_to_recursive_record);

	if (sub_node_content && sub_node_content == node)
		return (sub_node_content);
	return (NULL);
}

void					*find_serious_allocated_node(struct s_node *node)
{
	struct s_record *record;
	struct s_index *index;
	enum e_page_type type;

	record = btree_get_node_content(node);
	ft_printf("Warning: find_serious_allocated_node root founded !\n");

	type = record->size <= TINY_RANGE ? TINY: MEDIUM;
	type = get_page_type(record->size);
	if (type == LARGE)
		return (NULL);
	index = (struct s_index *)btree_search_content(
			(type == TINY) ?
			ctx.tiny_index_pages_tree : ctx.medium_index_pages_tree,
			record,
			(type == TINY) ? &cmp_record_addr_to_tiny_index : &cmp_record_addr_to_medium_index);
	if (index && index->allocation_tree == node)
			return (&index->allocation_tree);
	return (NULL);
}

void					*find_serious_root(struct s_node *node)
{
	void *ptr;

	if (node == ctx.tiny_index_pages_tree)
		return (&ctx.tiny_index_pages_tree);
	else if (node == ctx.medium_index_pages_tree)
		return (&ctx.medium_index_pages_tree);
	else if (node == ctx.big_page_record_tree)
		return (&ctx.big_page_record_tree);
	else if (node == ctx.global_tiny_space_tree)
	{
		ft_printf("moving global_tiny_space_tree\n");
		return (&ctx.global_tiny_space_tree);
	}
	else if (node == ctx.global_medium_space_tree)
		return (&ctx.global_medium_space_tree);

	ptr = find_serious_allocated_node(node);
	if (ptr != NULL)
		return (ptr);
	ptr = find_serious_free_node(node);
	if (ptr != NULL)
		return (ptr);

	ft_printf("root not founded !\n");
	return (NULL);
}

void					node_custom_deallocator(void *node)
{
	struct s_node_page	*end_node_page;
	uint8_t				*src_node;

	//return;

	ft_printf("BEGIN OF DEALLOCATION\n");
	ft_printf("node_custom: delete node at: %p\n", node);
	end_node_page = ctx.node_pages_entry;
	assert(end_node_page->primary_block.nb_node > 0);
	src_node = (uint8_t *)(&end_node_page->node) +
			((end_node_page->primary_block.nb_node - 1) * btree_get_node_size());

	ft_printf("moving node, src_node = %p\n", src_node);




	if (node != (void *)src_node)
	{
		if (((struct s_node *)src_node)->parent == NULL)
		{
			ft_printf("ROOT HAS CHANGED !\n");
			find_root(src_node);
			uint64_t *ptr;
			ptr = (uint64_t *)find_serious_root((struct s_node *)src_node);
			*ptr = (uint64_t)node;
		}
		ft_printf("call to btree_memory_move\n");
		btree_memory_move(node, (struct s_node *)src_node);
	}

	//ft_bzero(src_node, btree_get_node_size());


	end_node_page->primary_block.nb_node -= 1;
	if (end_node_page->primary_block.nb_node == 0 &&
			end_node_page->primary_block.next != NULL)
	{
		ft_printf("Destroying node page\n");
		ctx.node_pages_entry = end_node_page->primary_block.next;
		destroy_pages(end_node_page, NODE_REQ_PAGES * ctx.page_size);
	}

	find_root(src_node);
	ft_printf("END OF DEALLOCATION\n");
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
