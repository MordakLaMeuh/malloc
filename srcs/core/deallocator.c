/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deallocator.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

static void			restitute_space(
		struct s_node		*node,
		struct s_record		*record,
		struct s_index		*index
)
{
	(void)node;
	(void)record;
	(void)index;
	return ;
}

/*
** Do nothing when specify a bas address, just return, not exit.
*/

void				core_deallocator(void *addr)
{
	struct s_node		*node;
	struct s_record		*record;
	struct s_index		*index;

	if ((node = search_record_node((uint64_t)addr, &index)) == NULL)
		return ;
	record = (struct s_record *)btree_get_node_content(node);
	if (record == NULL)
		return ;
	restitute_space(node, record, index);
	btree_delete_rnb_node(&index->allocation_tree, node,
			&node_custom_deallocator);
	return ;
}
