/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"
#include <unistd.h>

void				*ft_malloc(size_t size)
{
	struct s_record	*record;
	void *addr;

#ifdef DEBUG_INFO
	ft_putstr("custom malloc called !\n");
#endif
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
		return (NULL);
	}
	record->addr = (uint64_t)addr;
	record->size = size;
#ifdef DEBUG_INFO
	printf("generating addr to %p\n", (void *)record->addr);
	ft_putstr("custom malloc success\n");
#endif
	return (addr);
}
