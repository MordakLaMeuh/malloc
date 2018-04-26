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
** Claim pages from Kernel, size may be calibrated to page_size.
*/

void		*get_new_pages(size_t size)
{
	void *new_page;

	new_page = mmap(
		NULL,
		size,
		PROT_READ | PROT_WRITE,
#ifdef __APPLE__
		MAP_ANON | MAP_PRIVATE,
#else
		MAP_ANONYMOUS | MAP_PRIVATE,
#endif
		-1,
		0);
#ifdef DEBUG_PAGES
	printf("* NEW PAGE ALLOCATED size: %lu addr: %p *\n", size, new_page);
#endif
	return (new_page == MAP_FAILED) ? NULL : new_page;
}

/*
** Say to lernel to destroy pages, size may be calibrated to page_size.
*/

int			destroy_pages(void *addr, size_t size)
{
#ifdef DEBUG_PAGES
	printf("* DESTROYING PAGE nb: %lu addr: %p *\n", size, addr);
#endif
	return (munmap(
		addr,
		size));
}
