/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static void		assign_parent_free_tiny(
		void *content,
		struct s_node *node)
{
	size_t *size;

	size = (size_t *)content;
	node->size = *size;
	node->content = NULL;
	node->mask.s.node_type = PARENT_RECORD_FREE_TINY;
}

static void		assign_parent_free_medium(
		void *content,
		struct s_node *node)
{
	size_t *size;

	size = (size_t *)content;
	node->size = *size;
	node->content = NULL;
	node->mask.s.node_type = PARENT_RECORD_FREE_MEDIUM;
}

struct s_node	*insert_free_record(
		void *addr,
		size_t size,
		enum e_page_type type,
		struct s_node **parent_ref)
{
	struct s_node			**root;
	struct s_node			*parent;
	struct s_node			*record;
	struct s_node_params	use_ctx;

	root = (type == TINY) ?
			&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree;
	use_ctx.allocator = &node_custom_allocator;
	use_ctx.comp = &cmp_size_to_node_size;
	use_ctx.associator = (type == TINY) ?
			&assign_parent_free_tiny : &assign_parent_free_medium;
	parent = btree_try_to_insert_rnb_node(root, &size, &use_ctx);
	if (parent == NULL)
		return (NULL);
	if (parent_ref)
		*parent_ref = parent;
	record = btree_create_node(&node_custom_allocator);
	assert(record != NULL);
	if (record == NULL)
		return (NULL);
	record->size = size;
	record->content = addr;
	record->mask.s.node_type = (type == TINY) ?
			RECORD_FREE_TINY : RECORD_FREE_MEDIUM;
	ft_printf("{blue}Inserting free node: addr=%p size=%lu parent=%p{eoc}\n", addr, size, parent);
	record = btree_insert_rnb_node(((struct s_node **)&parent->content),
			record, &cmp_node_addr_to_node_addr);
	return (record);
}

struct s_node	*get_free_record(
		void *addr,
		size_t size,
		struct s_node **parent,
		enum e_page_type type)
{
	struct s_node	*root;
	struct s_node	*out;

	root = (type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree;
	*parent = btree_get_node_by_content(root, &size, &cmp_size_to_node_size);
	if (*parent == NULL)
		return (NULL);
	out = btree_get_node_by_content(((struct s_node *)(*parent)->content)
			, addr, &cmp_addr_to_node_addr);
	if (out == NULL)
		return (NULL);
	return (out);
}

int				delete_free_record(
		struct s_node *record,
		struct s_node *parent,
		enum e_page_type type)
{
	struct s_node			**root;
	size_t					size;
	int						ret;

	root = (type == TINY) ?
			&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree;
	size = record->size,
	ret = btree_delete_rnb_node(((struct s_node **)&parent->content),
			record, &node_custom_deallocator);
	if (ret == 0)
	{
		btree_delete_rnb_node_by_content(
				root,
				&size,
				&cmp_size_to_node_size,
				&node_custom_deallocator);
	}
	return (0);
}

struct s_node	*get_best_free_record_tree(
		size_t size,
		enum e_page_type type)
{
	struct s_node	**root;
	struct s_node	*parent;
	struct s_node	*index;
	void			*addr;

	root = (type == TINY) ?
			&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree;
	parent = btree_get_last_valid_node(*root, &size, &cmp_size_to_node_size);
	if (parent == NULL)
	{
		addr = get_new_pages(type == TINY ? TINY_RANGE : MEDIUM_RANGE);
		if (addr == NULL)
			return (NULL);
		index = create_index(addr, type);
		if (index == NULL)
		{
			destroy_pages(addr, type == TINY ? TINY_RANGE : MEDIUM_RANGE);
			return (NULL);
		}
		if (insert_free_record(addr, type == TINY ? TINY_RANGE : MEDIUM_RANGE,
				type, &parent) == NULL)
		{
			destroy_pages(addr, type == TINY ? TINY_RANGE : MEDIUM_RANGE);
			destroy_index(index, type);
			return (NULL);
		}
	}
	return (parent);
}
