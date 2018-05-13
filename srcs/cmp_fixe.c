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

int		cmp_addr_to_node_addr(
		void *addr,
		struct s_node *node)
{
	if (addr < node->content)
		return (-1);
	if (addr > node->content)
		return (1);
	return (0);
}

int		cmp_node_addr_to_node_addr(
		struct s_node *node_a,
		struct s_node *node_b)
{
	if (node_a->content < node_b->content)
		return (-1);
	if (node_a->content > node_b->content)
		return (1);
	return (0);
}

int		cmp_size_to_node_size(
		void *size,
		struct s_node *node)
{
	size_t *len;

	len = (size_t *)size;
	if (*len < node->size)
		return (-1);
	if (*len > node->size)
		return (1);
	return (0);
}

int		cmp_node_size_to_node_size(
		struct s_node *node_a,
		struct s_node *node_b)
{
	if (node_a->size < node_b->size)
		return (-1);
	if (node_a->size > node_b->size)
		return (1);
	return (0);
}
