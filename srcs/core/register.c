/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 17:50:30 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

int				add_reg_entry(void *addr, size_t size)
{
	struct s_reg_page *reg_page;

	reg_page = (struct s_reg_page *)get_new_pages(1);
	(void)reg_page;
	(void)addr;
	(void)size;
	return (0);
}

int				del_reg_entry(void *addr)
{
	(void)addr;
	return (0);
}
