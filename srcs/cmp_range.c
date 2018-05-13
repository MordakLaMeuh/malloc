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

int		cmp_addr_to_node_size_tiny_range(
		void *content,
		struct s_node *node)
{
	uint64_t record_addr;

	record_addr = (uint64_t)content;
	if (record_addr < (uint64_t)node->size)
		return (-1);
	if (record_addr >= (uint64_t)node->size + TINY_RANGE)
		return (1);
	return (0);
}

int		cmp_addr_to_node_size_medium_range(
		void *content,
		struct s_node *node)
{
	uint64_t record_addr;

	record_addr = (uint64_t)content;
	if (record_addr >= (uint64_t)node->size + MEDIUM_RANGE)
		return (-1);
	if (record_addr < node->size)
		return (1);
	return (0);
}
