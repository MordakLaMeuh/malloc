/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btree_rotation_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/22 13:16:59 by bmickael          #+#    #+#             */
/*   Updated: 2017/03/22 13:34:46 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "btree_internal_header.h"

/*
** See this video "Red-black trees in 3 minutes — Rotations" to understand.
** This below rotations swap voluntary the content, we don't have to
** manually color node in insertion strategy if we use them.
*/

void	btree_rotate_left(struct s_node *n)
{
	void			*content;
	struct s_node	*right_child;

	right_child = n->right;
	content = n->content;
	n->content = right_child->content;
	right_child->content = content;
	n->right = right_child->right;
	right_child->right = right_child->left;
	right_child->left = n->left;
	n->left = right_child;
	if (n->right)
		n->right->parent = n;
	if (right_child->left)
		right_child->left->parent = right_child;
}

void	btree_rotate_right(struct s_node *n)
{
	void			*content;
	struct s_node	*left_child;

	left_child = n->left;
	content = n->content;
	n->content = left_child->content;
	left_child->content = content;
	n->left = left_child->left;
	left_child->left = left_child->right;
	left_child->right = n->right;
	n->right = left_child;
	if (n->left)
		n->left->parent = n;
	if (left_child->right)
		left_child->right->parent = left_child;
}

/*
** This below rotations are pure pointer redirection, none color changed.
** Use it for deletion.
*/

void	btree_delete_rotate_right(struct s_node *node)
{
	struct s_node *left_child;

	left_child = node->left;
	left_child->parent = node->parent;
	if (node->parent)
	{
		if (node->parent->left == node)
			node->parent->left = left_child;
		else
			node->parent->right = left_child;
	}
	node->left = left_child->right;
	if (node->left)
		node->left->parent = node;
	left_child->right = node;
	node->parent = left_child;
}

void	btree_delete_rotate_left(struct s_node *node)
{
	struct s_node *right_child;

	right_child = node->right;
	right_child->parent = node->parent;
	if (node->parent)
	{
		if (node->parent->left == node)
			node->parent->left = right_child;
		else
			node->parent->right = right_child;
	}
	node->right = right_child->left;
	if (node->right)
		node->right->parent = node;
	right_child->left = node;
	node->parent = right_child;
}
