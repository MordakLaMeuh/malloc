/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
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

void			ft_free(void *ptr)
{
	struct s_record	*record;
	int				ret;

#ifdef DEBUG_INFO
	ft_putstr("custom free called !\n");
#endif
	record = search_record(ptr);
	if (record == NULL) {
		ft_putstr_fd("Double free or corruption\n", STDERR_FILENO);
		exit (1);
	}

#ifdef DEBUG_INFO
	ft_putstr("founded !\n");
#endif
	ret = del_record(record);
	if (ret < 0) {
		ft_putstr_fd("Unexpected error\n", STDERR_FILENO);
		exit (1);
	}

#ifdef DEBUG_INFO
	ft_putstr("custom free success\n");
#endif
}
