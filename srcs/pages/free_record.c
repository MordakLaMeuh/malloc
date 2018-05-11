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

static int		cmp_sub_addr(void *r1, void *r2)
{
	uint64_t		*test_addr;

	test_addr = (uint64_t *)r1;
	if (*test_addr < ((struct s_record *)r2)->addr)
		return (-1);
	if (*test_addr > ((struct s_record *)r2)->addr)
		return (1);
	return (0);
}

static int		cmp_sub_size_to_recursive_record(void *size, void *record_node)
{
	struct s_record *record;
	uint64_t *length;

	length = (uint64_t *)size;
	record = btree_get_node_content(record_node);
	ft_printf("lenght = %lu, record_size = %lu\n", *length, record->size);
	if (*length < record->size)
		return (-1);
	if (*length > record->size)
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

	ft_printf("Trash free record\n");
	size_node = btree_get_node_by_content(
			(type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&size, &cmp_sub_size_to_recursive_record);
	assert(size_node != NULL);
	sub_root = btree_get_node_content(size_node);
	recurse_delete = btree_is_last_node(sub_root) ? true : false;
	btree_delete_rnb_node_by_content(&sub_root, &addr,
			&cmp_sub_addr, &node_custom_deallocator);
	if (recurse_delete)
	{
		ft_printf("recurse delete activating\n");
		btree_delete_rnb_node(type == TINY ?
				&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree,
				size_node, &node_custom_deallocator);
		ft_printf("new tiny space addr = %p\n", ctx.global_tiny_space_tree);
	}
	else
		ft_printf("no recuse delete\n");
}


static int		custom_cmp_sub_size(void *record_node, void *sub_node)
{
	struct s_record *record;
	struct s_record *sub_node_record;

	record = (struct s_record *)btree_get_node_content(record_node);
	sub_node_record = (struct s_record *)btree_get_node_content(sub_node);

	if (record->size < sub_node_record->size)
		return (-1);
	if (record->size > sub_node_record->size)
		return (1);
	return (0);
}

static int		cmp_record_to_record(void *record1, void *record2)
{
	if (((struct s_record *)record1)->addr < ((struct s_record *)record2)->addr)
		return (-1);
	if (((struct s_record *)record1)->addr > ((struct s_record *)record2)->addr)
		return (1);
	return (0);
}

struct s_node	*add_free_record(struct s_record *record, enum e_page_type type)
{
	struct s_node	*size_node_tree;
	struct s_node	*record_node;

	ft_printf("ctx.global_tiny_space_tree = %p\n", ctx.global_tiny_space_tree);
	size_node_tree = btree_get_node_by_content(
			(type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&record->size, &cmp_sub_size_to_recursive_record);
	if (size_node_tree == NULL)
	{
		ft_printf("size_node_tree == NULL\n");
		record_node = btree_create_node(record, &node_custom_allocator);

		ft_printf("record->size = %lu, record_addr = %p\n", record->size, record->addr);
		struct s_record *rec = btree_get_node_content(record_node);
		ft_printf("record->size = %lu, record_addr = %p\n", rec->size, rec->addr);

		assert(record_node != NULL);


		size_node_tree = btree_insert_rnb_node_by_content((type == TINY) ?
				&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree,
				record_node, &custom_cmp_sub_size, &node_custom_allocator);


/*
		size_node_tree = btree_insert_rnb_node_by_content((type == TINY) ?
				&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree,
				&record->size, &cmp_record_to_recursive_record, &node_custom_allocator);
*/

		return (size_node_tree);
	}
	record_node = btree_get_node_content(size_node_tree);
	assert(record_node != NULL);
	btree_insert_rnb_node_by_content(
			&record_node, record, &cmp_record_to_record, &node_custom_allocator);
	return (size_node_tree);
}

struct s_node	*get_free_record_node(size_t size, enum e_page_type type)
{
	struct s_node *suitable_node;

	ft_printf("size requested: %lu\n", size);
	suitable_node = (struct s_node *)
			btree_get_last_valid_node((type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&size, &cmp_sub_size_to_recursive_record);
	if (suitable_node == NULL)
		ft_printf("NO space founded !\n");
	return (suitable_node);
}
