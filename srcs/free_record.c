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

static void	display_item(struct s_node *item, int lvl, int first_elem) {
	if (first_elem)
		ft_printf("%s--- level %i ---\n", (lvl != 0) ? "\n" : "", lvl);
	char color;

	switch (item->mask.s.color)
	{
	case (BLACK):
		color = 'B';
		break;
	case (RED):
		color = 'R';
		break;
	case (DOUBLE_BLACK):
		color = 'D';
		break;
	default:
		color = 'O';
	}
	ft_printf("[%c %lu", color, item->m.size);
	if (item->parent)
		ft_printf(" (%lu)", item->parent->m.size);
	ft_printf("]  ");
}

void		debug_free_record(enum e_page_type type)
{
	if (B_DEBUG == 0)
		return ;
	ft_printf("\n");
	btree_apply_by_level((type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree, &display_item);
	ft_printf("\n");
	struct s_rnb_tree_checker_result result;
	btree_check_rnb_property((type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree, &result);
	ft_printf("root_is_black      : %s\n", result.root_is_black == FAILED ? "FAIL" : "OK");
	ft_printf("homogenetic_black  : %s\n", result.homogenetic_black == FAILED ? "FAIL" : "OK");
	ft_printf("filiation          : %s\n", result.filiation == FAILED ? "FAIL" : "OK");
	ft_printf("rnb_interlacement  : %s\n", result.rnb_interlacement == FAILED ? "FAIL" : "OK");
	ft_printf("%i levels for %i nodes\n", result.nb_levels, result.nb_nodes);
}

static void		assign_parent_free_tiny(
		void *content,
		struct s_node *node)
{
	size_t *size;

	size = (size_t *)content;
	node->m.size = *size;
	node->ptr_a = NULL;
	node->mask.s.node_type = PARENT_RECORD_FREE_TINY;
}

static void		assign_parent_free_medium(
		void *content,
		struct s_node *node)
{
	size_t *size;

	size = (size_t *)content;
	node->m.size = *size;
	node->ptr_a = NULL;
	node->mask.s.node_type = PARENT_RECORD_FREE_MEDIUM;
}

int				check_index_destroy(
		void *addr,
		size_t size,
		enum e_page_type type)
{
	struct s_node *parent;

	parent = NULL;
	if (type == TINY && size == TINY_RANGE)
		parent = btree_get_node_by_content(
				ctx.global_tiny_space_tree,
				&size, &cmp_size_to_node_size);
	if (type == MEDIUM && size == MEDIUM_RANGE)
		parent = btree_get_node_by_content(
				ctx.global_medium_space_tree,
				&size, &cmp_size_to_node_size);
	if (parent)
	{
		destroy_index(
				btree_get_node_by_content(
						ctx.index_pages_tree,
						addr,
						&cmp_m_addr_to_node_m_addr));
		return (1);
	}
	return (0);
}

int				insert_free_record(
		void *addr,
		size_t size,
		enum e_page_type type,
		struct s_node **parent_ref)
{
	struct s_node			*parent;
	struct s_node			*record;
	struct s_node_params	use_ctx;

	if (check_index_destroy(addr, size, type))
		return (0);

	use_ctx.allocator = &node_custom_allocator;
	use_ctx.comp = &cmp_size_to_node_size;
	use_ctx.associator = (type == TINY) ?
			&assign_parent_free_tiny : &assign_parent_free_medium;

	debug_free_record(type);

	parent = btree_try_to_insert_rnb_node((type == TINY) ?
			&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree, &size, &use_ctx);

	debug_free_record(type);

	if (parent == NULL)
		return (-1);
	if (parent_ref)
		*parent_ref = parent;
	record = btree_create_node(&node_custom_allocator);
	assert(record != NULL);
	if (record == NULL)
		return (-1);
	record->m.size = size;
	record->ptr_a = addr;
	record->mask.s.node_type = (type == TINY) ?
			RECORD_FREE_TINY : RECORD_FREE_MEDIUM;
	ft_dprintf(B_DEBUG, "{blue}Inserting free node: addr=%p size=%lu parent=%p{eoc}\n", addr, size, parent);
	record = btree_insert_rnb_node(((struct s_node **)&parent->ptr_a),
			record, &cmp_node_addr_to_node_addr);
	ft_dprintf(B_DEBUG, "ALLOCATION ENDED\n");
	return (0);
}

struct s_node	*get_free_record(
		void *addr,
		size_t size,
		struct s_node **parent,
		enum e_page_type type)
{
	struct s_node	*out;

	*parent = btree_get_node_by_content((type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree,
			&size, &cmp_size_to_node_size);
	if (*parent == NULL)
		return (NULL);
	out = btree_get_node_by_content(((struct s_node *)(*parent)->ptr_a)
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
	size_t					size;
	int						ret;

	size = record->m.size;
	ret = btree_delete_rnb_node(((struct s_node **)&parent->ptr_a),
			record, &node_custom_deallocator);
	if (ret == 0)
	{
		debug_free_record(type);

		btree_delete_rnb_node_by_content(
				(type == TINY) ?
							&ctx.global_tiny_space_tree : &ctx.global_medium_space_tree,
				&size,
				&cmp_size_to_node_size,
				&node_custom_deallocator);

		debug_free_record(type);
	}
	return (0);
}

struct s_node	*get_best_free_record_tree(
		size_t size,
		enum e_page_type type)
{
	struct s_node	*parent;
	struct s_node	*index;
	void			*addr;

	parent = btree_get_last_valid_node((type == TINY) ?
			ctx.global_tiny_space_tree : ctx.global_medium_space_tree, &size,
			&cmp_size_to_node_size);
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
				type, &parent) < 0)
		{
			destroy_pages(addr, type == TINY ? TINY_RANGE : MEDIUM_RANGE);
			destroy_index(index);
			return (NULL);
		}
	}
	return (parent);
}
