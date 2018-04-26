/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

#define BLOC_MASK 64

static uint32_t	get_required_sectors(size_t size, enum e_page_type page_type)
{
	uint32_t	required_sectors;
	int			shift_q;

	shift_q = (page_type == TINY) ? TINY_SHR : MEDIUM_SHR;
	required_sectors = size >> shift_q;
	if (size != required_sectors << shift_q)
		required_sectors += 1;
	return required_sectors;
}

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

static int		fill(uint64_t *field, uint32_t required_sectors)
{
	int			initial_offset;
	uint64_t	mask;
	uint64_t	r;
	int			offset;

	if (required_sectors == BLOC_MASK && *field != 0)
		return (-1);
	initial_offset = BLOC_MASK - required_sectors;
	mask = (((uint64_t)1 << required_sectors) - 1) << initial_offset;
	while (mask >> initial_offset != 0)
	{
		r = *field & mask;
		if (r == 0)
		{
			*field |= mask;
			return (BLOC_MASK - initial_offset - required_sectors);
		}
		offset = jump(r >> initial_offset, required_sectors);
		mask >>= offset;
		initial_offset -= offset;
	}
	return (-1);
}

static int		fill_index(struct s_index *index, int required_sectors)
{
	int			sector_founded;

	sector_founded = fill(&index->chunk_a, required_sectors);
	if (sector_founded != -1)
		return (sector_founded);
	sector_founded = fill(&index->chunk_b, required_sectors);
	if (sector_founded != -1)
		return (sector_founded + 1 * BLOC_MASK);
	sector_founded = fill(&index->chunk_c, required_sectors);
	if (sector_founded != -1)
		return (sector_founded + 2 * BLOC_MASK);
	sector_founded = fill(&index->chunk_d, required_sectors);
	if (sector_founded != -1)
		return (sector_founded + 3 * BLOC_MASK);
	return (-1);
}

static uint64_t	sector_to_addr(
	uint64_t base_addr,
	enum e_page_type page_type,
	uint32_t sector)
{
	base_addr += (sector * ((page_type == TINY) ?
		TINY_BLOCK_SIZE : MEDIUM_BLOCK_SIZE));
	return (base_addr);
}

static struct s_index	*get_new_index(void)
{
	struct s_index_page	*tmp;
	int					*nb_index;

	tmp = ctx.last_index_page;
	if (tmp == NULL || tmp->primary_block.nb_index == ctx.index_density)
	{
#ifdef DEBUG_INDEX
		printf("Creating new index_page\n");
#endif
		ctx.last_index_page =
			(struct s_index_page *)get_new_pages(ctx.page_size);
		if (ctx.last_index_page == NULL)
			return (NULL);
		if (ctx.first_index_page == NULL)
			ctx.first_index_page = ctx.last_index_page;
		nb_index = &ctx.last_index_page->primary_block.nb_index;
		ctx.last_index_page->primary_block.next = NULL;
		ctx.last_index_page->primary_block.prev = tmp;
		if (tmp != NULL)
			tmp->primary_block.next = ctx.last_index_page;
		*nb_index = 1;
	}
	else
	{
		nb_index = &ctx.last_index_page->primary_block.nb_index;
		*nb_index += 1;
	}
	return (&ctx.last_index_page->index[*nb_index - 1]);
}

#include <unistd.h>
#include <stdlib.h>

static uint64_t	create_new_index(
	uint32_t required_sectors,
	enum e_page_type page_type)
{
	struct s_index *index;
	int sector;

	index = get_new_index();
	if (index == NULL)
		return (0);
	index->chunk_a = 0;
	index->chunk_b = 0;
	index->chunk_c = 0;
	index->chunk_d = 0;
	index->type = page_type;
	index->page_addr = (uint64_t)
		get_new_pages((page_type == TINY) ? TINY_RANGE : MEDIUM_RANGE);
	if (index->page_addr == 0)
		return (0);
	sector = fill_index(index, required_sectors);
	if (sector == -1)
		return (0);
#ifdef DEBUG_INDEX
	printf("new index four chunk created\n");
#endif
	return (sector_to_addr(index->page_addr, page_type, sector));
}

uint64_t		assign_index(size_t size)
{
	struct s_index_page *index_page;
	enum e_page_type page_type;
	uint32_t required_sectors;
	int sector;
	int i;

	page_type = (size <= TINY_LIMIT) ? TINY : MEDIUM;
	required_sectors = get_required_sectors(size, page_type);
	index_page = ctx.first_index_page;
	while (index_page)
	{
		i = 0;
		while (i < index_page->primary_block.nb_index)
		{
			if (page_type == index_page->index[i].type &&
				((sector = fill_index(&index_page->index[i],
				required_sectors)) != -1))
				return (sector_to_addr(
					index_page->index[i].page_addr, page_type, sector));
			i++;
		}
		index_page = index_page->primary_block.next;
	}
	return (create_new_index(required_sectors, page_type));
}

static uint32_t	addr_to_sector(uint64_t addr, struct s_index *index)
{
	uint64_t offset_addr;

	offset_addr = addr - index->page_addr;
	offset_addr >>= (index->type == TINY) ? TINY_SHR : MEDIUM_SHR;
	return ((uint32_t)offset_addr);
}

static int		destroy_index(struct s_index *index)
{
	struct s_index_page		*last_index_page;
	struct s_index			*last_index;

#ifdef DEBUG_INDEX
	printf("destroy index called\n");
#endif
	last_index_page = ctx.last_index_page;
	if (last_index_page == NULL)
		return (-1);
	last_index = &last_index_page->index
		[last_index_page->primary_block.nb_index - 1];
	last_index_page->primary_block.nb_index -= 1;
	if (index != last_index)
		ft_aligned_memcpy(index, last_index, sizeof(struct s_index));
	if (last_index_page->primary_block.nb_index == 0)
	{
		if (last_index_page->primary_block.prev)
			last_index_page->primary_block.prev->primary_block.next = NULL;
		else
			ctx.first_index_page = NULL;
		ctx.last_index_page = last_index_page->primary_block.prev;
#ifdef DEBUG_INDEX
		printf("Destoying index_page\n");
#endif
		if (destroy_pages(last_index_page, ctx.page_size) < 0)
			return (-1);
	}
	return (0);
}

static int		unfill(
	uint64_t *field,
	uint32_t sector,
	uint32_t required_sectors)
{
	uint64_t mask;
	int initial_offset;

	initial_offset = BLOC_MASK - sector - required_sectors;
#ifdef DEBUG_INDEX
	printf("mask before unfilling: %.16lx for sector %u\n", *field, sector);
#endif
	mask = (((uint64_t)1 << required_sectors) - 1) << initial_offset;
	*field &= ~mask;
#ifdef DEBUG_INDEX
	printf("mask after unfilling: %.16lx\n", *field);
#endif
	return (0);
}

static int		unreg_mark(
	struct s_index_page *index_page,
	int offset,
	uint64_t addr,
	size_t size
)
{
	struct s_index *index;
	uint32_t sector;
	uint32_t required_sectors;

	index = &index_page->index[offset];
	sector = addr_to_sector(addr, index);
	required_sectors = get_required_sectors(size, index->type);
#ifdef DEBUG_INDEX
	printf("unreg launched for sector %u!\n", sector);
#endif
	if (sector < 64)
		unfill(&index->chunk_a, sector, required_sectors);
	else if (sector < 128)
		unfill(&index->chunk_b, sector - 64, required_sectors);
	else if (sector < 192)
		unfill(&index->chunk_c, sector - 128, required_sectors);
	else
		unfill(&index->chunk_d, sector - 192, required_sectors);
	if (index->chunk_a == 0 && index->chunk_b == 0 &&
		index->chunk_c == 0 && index->chunk_d == 0)
	{
		destroy_pages((void *)addr,
			(index->type == TINY) ? TINY_RANGE : MEDIUM_RANGE);
		return (destroy_index(index));
	}
	return (0);
}

int				del_index(uint64_t addr, size_t size)
{
	struct s_index_page *index_page;
	struct s_index *index;
	enum e_page_type page_type;
	uint64_t off_last;
	int i;

	page_type = (size <= TINY_LIMIT) ? TINY : MEDIUM;
	off_last = (page_type == TINY) ? TINY_RANGE : MEDIUM_RANGE;
	index_page = ctx.first_index_page;
	while (index_page)
	{
		i = 0;
		while (i < index_page->primary_block.nb_index)
		{
			index = &index_page->index[i];
			if (page_type == index->type &&
				addr >= index->page_addr && addr < index->page_addr + off_last)
					return (unreg_mark(index_page, i, addr, size));
			i++;
		}
		index_page = index_page->primary_block.next;
	}
	return (-1);
}
