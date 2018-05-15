/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp_range.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

int		cmp_addr_to_node_m_addr_tiny_range(
		void *content,
		struct s_node *node)
{
	if (content < node->m.ptr_b)
		return (-1);
	if ((uint8_t *)content >= (uint8_t *)node->m.ptr_b + TINY_RANGE)
		return (1);
	return (0);
}

int		cmp_addr_to_node_m_addr_medium_range(
		void *content,
		struct s_node *node)
{
	if (content < node->m.ptr_b)
		return (-1);
	if ((uint8_t *)content >= (uint8_t *)node->m.ptr_b + MEDIUM_RANGE)
		return (1);
	return (0);
}

int			cmp_node_m_addr_to_node_m_addr(
		struct s_node *node_a,
		struct s_node *node_b)
{
	if (node_a->m.ptr_b < node_b->m.ptr_b)
		return (-1);
	if (node_a->m.ptr_b > node_b->m.ptr_b)
		return (1);
	return (0);
}
