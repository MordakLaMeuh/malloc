/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_syscall.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 17:50:30 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"
#include <unistd.h>

#define MAP_ANONYMOUS	0x20

/*
** Claim pages from Kernel, size is calibrated to page_size.
*/

void		*get_new_pages(int nb)
{
	void *new_page;

	new_page = mmap(
		NULL,
		ctx.page_size * nb,
		PROT_READ | PROT_WRITE,
#ifdef __APPLE__
		MAP_ANON | MAP_PRIVATE,
#else
		MAP_ANONYMOUS | MAP_PRIVATE,
#endif
		-1,
		0);
	printf("* NEW PAGE ALLOCATED nb: %i addr: %p *\n", nb, new_page);
	return (new_page == MAP_FAILED) ? NULL : new_page;
}

/*
** Say to lernel to destroy pages, size is calibrated to page_size.
*/

int			destroy_pages(void *addr, int nb)
{
	printf("* DESTROYING PAGE nb: %i addr: %p *\n", nb, addr);
	return (munmap(
		addr,
		ctx.page_size * nb));
}
