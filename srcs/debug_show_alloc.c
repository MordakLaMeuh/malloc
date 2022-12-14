/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_show_alloc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static void		display_alloc(struct s_node *record)
{
	int fd;

	fd = (ctx.tracer_file_descriptor != -1) ?
			ctx.tracer_file_descriptor : STDOUT_FILENO;
	ft_fprintf(fd, "%p --> %p  %lu\n",
			record->ptr_a,
			(uint8_t *)record->ptr_a + record->m.size - 1,
			record->m.size);
}

static void		display_pages_alloc_tiny(struct s_node *index)
{
	int fd;

	if (index->mask.s.range != TINY_RANGE)
		return ;
	fd = (ctx.tracer_file_descriptor != -1) ?
			ctx.tracer_file_descriptor : STDOUT_FILENO;
	ft_fprintf(fd, "{yellow}PAGE: %p{eoc}\n", (void *)index->m.size);
	btree_apply_infix(
			(struct s_node *)index->ptr_a,
			&display_alloc);
}

static void		display_pages_alloc_medium(struct s_node *index)
{
	int fd;

	if (index->mask.s.range != MEDIUM_RANGE)
		return ;
	fd = (ctx.tracer_file_descriptor != -1) ?
			ctx.tracer_file_descriptor : STDOUT_FILENO;
	ft_fprintf(fd, "{yellow}PAGE: %p{eoc}\n", (void *)index->m.size);
	btree_apply_infix(
			(struct s_node *)index->ptr_a,
			&display_alloc);
}

static void		display_pages_free(struct s_node *index)
{
	int fd;

	fd = (ctx.tracer_file_descriptor != -1) ?
			ctx.tracer_file_descriptor : STDOUT_FILENO;
	ft_fprintf(fd, "{yellow}chunk size: %lu{eoc}\n", (void *)index->m.size);
	btree_apply_infix(
			(struct s_node *)index->ptr_a,
			&display_alloc);
}

void			show_alloc(bool verbose, int fd)
{
	if (verbose)
	{
		debug_nodes(fd);
		ft_fprintf(fd, "\n{green}__TINY_FREE_BLOCK__{eoc}\n");
		btree_apply_infix(ctx.global_tiny_space_tree, &display_pages_free);
		ft_fprintf(fd, "\n{green}__MEDIUM_FREE_BLOCK__{eoc}\n");
		btree_apply_infix(ctx.global_medium_space_tree, &display_pages_free);
		ft_fprintf(fd, "\n");
	}
	ft_fprintf(fd, "{magenta}__TINY_ALLOCATED_BLOCK__{eoc}\n");
	btree_apply_infix(ctx.index_pages_tree, &display_pages_alloc_tiny);
	ft_fprintf(fd, "\n{magenta}__MEDIUM_ALLOCATED_BLOCK__{eoc}\n");
	btree_apply_infix(ctx.index_pages_tree, &display_pages_alloc_medium);
	ft_fprintf(fd, "\n{magenta}__LARGE_ALLOCATED_BLOCK__{eoc}\n");
	btree_apply_infix(ctx.big_page_record_tree, &display_alloc);
	ft_fprintf(fd, "\n{yellow}%lu{eoc} bytes allocated for user data,"
			"{yellow} %lu{eoc} bytes"
			" allocated by metadata nodes, total {magenta}%lu{eoc} bytes.\n\n",
			ctx.size_owned_by_data, ctx.size_owned_by_nodes,
			ctx.size_owned_by_data + ctx.size_owned_by_nodes);
}
