/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index_add.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

/*
** Test the filling of four chunks.
*/

static int		fill_index(struct s_index *index, int required_sectors)
{
	int			sector_founded;

	sector_founded = fill(&index->chunk_a, required_sectors);
	if (sector_founded != -1)
		return (sector_founded);
	sector_founded = fill(&index->chunk_b, required_sectors);
	if (sector_founded != -1)
		return (sector_founded + 1 * BLOC_COUNT);
	sector_founded = fill(&index->chunk_c, required_sectors);
	if (sector_founded != -1)
		return (sector_founded + 2 * BLOC_COUNT);
	sector_founded = fill(&index->chunk_d, required_sectors);
	if (sector_founded != -1)
		return (sector_founded + 3 * BLOC_COUNT);
	return (-1);
}

/*
** Request a new index field in index_page, create a new index page if necessary.
*/

static struct s_index	*get_new_index(void)
{
	struct s_index_page	*tmp;
	int					*nb_index;

	tmp = ctx.last_index_page;
	if (tmp == NULL || tmp->primary_block.nb_index == ctx.index_density)
	{
		ctx.last_index_page =
			(struct s_index_page *)get_new_pages(ctx.page_size);
		if (ctx.last_index_page == NULL)
			return (NULL);
		if (ctx.first_index_page == NULL)
			ctx.first_index_page = ctx.last_index_page;
		nb_index = &ctx.last_index_page->primary_block.nb_index;
		ctx.last_index_page->primary_block.prev = tmp;
		*nb_index = 1;
	}
	else
	{
		nb_index = &ctx.last_index_page->primary_block.nb_index;
		*nb_index += 1;
	}
	return (&ctx.last_index_page->index[*nb_index - 1]);
}

/*
** Create a proper index field, and DATA page associated.
*/

static uint64_t	create_new_index(
	uint32_t required_sectors,
	enum e_page_type page_type)
{
	struct s_index *index;
	void *addr;

	addr = get_new_pages((page_type == TINY) ? TINY_RANGE : MEDIUM_RANGE);
    if (addr == NULL)
    {
    	return (0);
    }
	index = get_new_index();
	if (index == NULL)
	{
	    destroy_pages(addr, (page_type == TINY) ? TINY_RANGE : MEDIUM_RANGE);
		return (0);
	}
	index->chunk_a = (1 << required_sectors) - 1;
	index->chunk_b = 0;
	index->chunk_c = 0;
	index->chunk_d = 0;
	index->type = page_type;
	index->page_addr = (uint64_t)addr;
	return (index->page_addr);
}

/*
** Try to fill nez sectors. If it can't, create a new index field.
*/

uint64_t		assign_index(size_t size)
{
	struct s_index_page *index_page;
	enum e_page_type page_type;
	uint32_t required_sectors;
	int sector;
	int i;

	page_type = (size <= TINY_LIMIT) ? TINY : MEDIUM;
	required_sectors = get_required_sectors(size, page_type);
	index_page = ctx.last_index_page;
	while (index_page)
	{
		i = index_page->primary_block.nb_index - 1;
		while (i != -1)
		{
			if (page_type == index_page->index[i].type &&
				((sector = fill_index(&index_page->index[i],
				required_sectors)) != -1))
				return (sector_to_addr(
					index_page->index[i].page_addr, page_type, sector));
			i--;
		}
		index_page = index_page->primary_block.prev;
	}
	return (create_new_index(required_sectors, page_type));
}

int				try_new_field(
	struct s_index *index,
	uint32_t sector,
	uint32_t required_sectors_old,
	uint32_t required_sectors_new)
{
	uint64_t *tab[4] = {&index->chunk_a, &index->chunk_b,
		&index->chunk_c, &index->chunk_d};
	uint64_t old_mask;
	uint64_t new_mask;
	int idx;

	idx = sector >> BLOC_COUNT_SHR;
	if (idx + required_sectors_new > BLOC_COUNT)
	    return (0);
	old_mask = (((uint64_t)1 << required_sectors_old) - 1) << (sector & 0x3f);
	*tab[idx] &= ~old_mask;
	new_mask = (((uint64_t)1 << required_sectors_new) - 1) << (sector & 0x3f);
	if ((*tab[idx] & new_mask) == 0)
	{
		*tab[idx] |= new_mask;
		return (1);
	}
	*tab[idx] |= old_mask;
	return (0);
}
