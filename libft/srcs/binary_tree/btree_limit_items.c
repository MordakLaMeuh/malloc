/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_limit_items.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/22 13:38:18 by bmickael          #+#    #+#             */
/*   Updated: 2017/03/24 07:12:30 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "btree_internal_header.h"

/*
** Logically, if the tree is okay, highest item is in far right sector.
*/

struct s_node	*get_highest_elem(struct s_node *node)
{
	while (node->right)
		node = node->right;
	return (node);
}

/*
** Logically, if the tree is okay, lowest item is in far left sector.
*/

struct s_node	*get_lowest_elem(struct s_node *node)
{
	while (node->left)
		node = node->left;
	return (node);
}
