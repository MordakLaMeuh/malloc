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
#include <stdlib.h>

void			*ft_malloc(size_t size)
{
	struct s_record *record;

#ifdef DEBUG_INFO
	ft_putstr("custom malloc called !\n");
#endif
	record = get_new_record();
	if (record == NULL) {
		ft_putstr_fd("Cannot allocate new record page\n", STDERR_FILENO);
		exit (1);
	}
	record->addr = (void *)(uint64_t)rand();
#ifdef DEBUG_INFO
	printf("generating addr to %p\n", record->addr);
#endif
	record->size = (uint32_t)size >> 4;
#ifdef DEBUG_INFO
	ft_putstr("custom malloc success\n");
#endif
	return record->addr;
}
