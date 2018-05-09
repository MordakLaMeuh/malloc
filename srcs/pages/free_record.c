/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_record.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static int		cmp_sub_size(void *r1, void *r2)
{
	struct s_node	*sub_tree;
	size_t			sub_size;
	size_t			*test_size;

	sub_tree = btree_get_node_content(r2);
	sub_size = ((struct s_record *)btree_get_node_content(sub_tree))->size;
	test_size = (size_t *)r1;
	if (*test_size < sub_size)
		return (-1);
	if (*test_size > sub_size)
		return (1);
	return (0);
}

static int		cmp_sub_addr(void *r1, void *r2)
{
	struct s_node	*sub_tree;
	size_t			sub_size;
	uint64_t		*test_addr;

	sub_tree = btree_get_node_content(r2);
	sub_size = ((struct s_record *)btree_get_node_content(sub_tree))->size;
	test_addr = (uint64_t *)r1;
	if (*test_addr < ((struct s_record *)r2)->addr)
		return (-1);
	if (*test_addr > ((struct s_record *)r2)->addr)
		return (1);
	return (0);
}

void			trash_free_record(
		uint64_t addr,
		size_t size,
		enum e_page_type type)
{
	struct s_node	*size_node;
	struct s_node	*sub_root;
	bool			recurse_delete;

	size_node = btree_get_node_by_content(
			(type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&size, &cmp_sub_size);
	assert(size_node != NULL);
	sub_root = btree_get_node_content(size_node);
	recurse_delete = btree_is_last_node(sub_root) ? true : false;
	btree_delete_rnb_node_by_content(&sub_root, &addr,
			&cmp_sub_addr, &node_custom_deallocator);
	if (recurse_delete)
		btree_delete_rnb_node(type == TINY ?
				&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree,
				size_node, &node_custom_deallocator);
}

void			add_free_record(struct s_record *record, enum e_page_type type)
{
	struct s_node	*size_node;
	struct s_node	*record_node;

	size_node = btree_get_node_by_content(
			(type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&record->size, &cmp_sub_size);
	if (size_node == NULL)
	{
		record_node = btree_create_node(record, &node_custom_allocator);
		assert(record_node != NULL);
		btree_insert_rnb_node_by_content((type == TINY) ?
				&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree,
				&record->size, &cmp_sub_size, &node_custom_allocator);
		return ;
	}
	record_node = btree_get_node_content(size_node);
	assert(record_node != NULL);
	btree_insert_rnb_node_by_content(
			&record_node, record, &cmp_record_addr, &node_custom_allocator);
}

struct s_node	*get_free_record_node(size_t size, enum e_page_type type)
{
	struct s_node *suitable_node;

	suitable_node = (struct s_node *)
			btree_get_last_valid_node((type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&size, &cmp_sub_size);
	return (suitable_node);
}
