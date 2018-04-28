/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

/*
** Get the number of sector required associated to a specfic size
** and page_type.
*/

uint32_t		get_required_sectors(size_t size, enum e_page_type page_type)
{
	int			shift_q;

	shift_q = (page_type == TINY) ? TINY_SHR : MEDIUM_SHR;
	return (size >> shift_q);
}

/*
** Convert the n sector to a real address.
**	base_addr: Address of the beginning of page.
**	page_type: Is-it tiny or medium page ?
**	sector: n sector from 0 to 255.
*/

uint64_t		sector_to_addr(
	uint64_t base_addr,
	enum e_page_type page_type,
	uint32_t sector)
{
	base_addr += (sector * ((page_type == TINY) ?
		TINY_BLOCK_SIZE : MEDIUM_BLOCK_SIZE));
	return (base_addr);
}

/*
** Convert an address to n sector.
*/

uint32_t		addr_to_sector(uint64_t addr, struct s_index *index)
{
	uint64_t offset_addr;

	offset_addr = addr - index->page_addr;
	offset_addr >>= (index->type == TINY) ? TINY_SHR : MEDIUM_SHR;
	return ((uint32_t)offset_addr);
}

/*
** Introduce a binary mechanism to skip some bit fail tests.
*/

static int		jump(uint64_t r)
{
	static int	itab[16] = {0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};
	int			i;

	i = 0;
	while (r >> 4)
	{
		i++;
		r >>= 4;
	}
	return (itab[r & 0xf] + (i * 4));
}

/*
** Fill a chunk.
** Return sector number, or -1 if fail.
** XXX Caution: Never use this function with required sector >= BLOC_COUNT
*/

int				fill(uint64_t *field, uint32_t required_sectors)
{
	int			initial_offset;
	uint64_t	mask;
	uint64_t	r;
	int			offset;

	initial_offset = 0;
	mask = (((uint64_t)1 << required_sectors) - 1) << initial_offset;
	while (initial_offset + required_sectors <= BLOC_COUNT)
	{
		r = *field & mask;
		if (r == 0)
		{
			*field |= mask;
			return (initial_offset);
		}
		offset = jump(r >> initial_offset);
		mask <<= offset;
		initial_offset += offset;
	}
	return (-1);
}
