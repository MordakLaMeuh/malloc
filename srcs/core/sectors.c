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
	uint32_t	required_sectors;
	int			shift_q;

	shift_q = (page_type == TINY) ? TINY_SHR : MEDIUM_SHR;
	required_sectors = size >> shift_q;
	if (size != required_sectors << shift_q)
		required_sectors += 1;
	return required_sectors;
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

static int		jump(uint64_t r, uint32_t required_sectors)
{
	static int	itab[16] = {4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0};
	uint32_t	tmp;

	while (1)
	{
		tmp = r & 0xf,
		required_sectors -= itab[tmp];
		if (tmp != 0)
			break;
		r >>= 4;
	}
	return (required_sectors);
}

/*
** Fill a chunk.
*/

int				fill(uint64_t *field, uint32_t required_sectors)
{
	int			initial_offset;
	uint64_t	mask;
	uint64_t	r;
	int			offset;

	if (required_sectors == BLOC_COUNT && *field != 0)
		return (-1);
	initial_offset = BLOC_COUNT - required_sectors;
	mask = (((uint64_t)1 << required_sectors) - 1) << initial_offset;
	while (mask >> initial_offset != 0)
	{
		r = *field & mask;
		if (r == 0)
		{
			*field |= mask;
			return (BLOC_COUNT - initial_offset - required_sectors);
		}
		offset = jump(r >> initial_offset, required_sectors);
		mask >>= offset;
		initial_offset -= offset;
	}
	return (-1);
}
