/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   class.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "dyn_allocator.h"

void	before_start(void)
{
	ctx.page_size = (size_t)getpagesize();
	ctx.idx_page_count = 0;
	ctx.first_idx_page = NULL;
 	printf("size = %lu\n", sizeof(struct s_idx_page_description));
}
