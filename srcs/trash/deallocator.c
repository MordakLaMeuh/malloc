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

/*
** Do nothing when specify a bas address, just return, not exit.
*/

void				core_deallocator(void *addr)
{
	struct s_record *record;
	int				ret;

	if ((record = search_record((uint64_t)addr)) == NULL)
		return ;
	if (record->size <= MEDIUM_LIMIT)
		ret = del_index(record->addr, record->size);
	else
		ret = destroy_pages((void *)record->addr, record->size);
	if (ret < 0)
	{
		ft_putstr_fd("Unexpected error at del index", STDERR_FILENO);
		exit(1);
	}
	ret = del_record(record);
	if (ret < 0)
	{
		ft_putstr_fd("Unexpected error at del record", STDERR_FILENO);
		exit(1);
	}
}
