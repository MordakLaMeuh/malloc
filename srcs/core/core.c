/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

void				*core_allocator(size_t size)
{
	struct s_record	*record;
	void			*addr;

	if (size <= MEDIUM_LIMIT)
		addr = (void *)assign_index(size);
	else
		addr = get_new_pages(size);
	if (addr == NULL) {
		ft_putstr_fd("Cannot allocate new page\n", STDERR_FILENO);
		return (NULL);
	}
	record = get_new_record();
	if (record == NULL) {
		ft_putstr_fd("Cannot allocate new record page\n", STDERR_FILENO);
		if (size <= MEDIUM_LIMIT)
			del_index((uint64_t)addr, size);
		else
			destroy_pages(addr, size);
		return (NULL);
	}
	record->addr = (uint64_t)addr;
	record->size = size;
	return addr;
}
