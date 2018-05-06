/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index_del.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

/*
** Destroy an index field, even an index page if necessary.
*/

static int			destroy_index(struct s_index *index)
{
	struct s_index_page		*last_index_page;
	struct s_index			*last_index;

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
		if (last_index_page->primary_block.prev == NULL)
			ctx.first_index_page = NULL;
		ctx.last_index_page = last_index_page->primary_block.prev;
		if (destroy_pages(last_index_page, ctx.page_size) < 0)
			return (-1);
	}
	return (0);
}

/*
** Find the good place to unfill sectors.
*/

static int			unreg_mark(
	struct s_index *index,
	uint64_t addr,
	size_t size
)
{
	uint64_t	*tab[4];
	uint32_t	sector;
	uint32_t	required_sectors;
	uint64_t	mask;

	tab[0] = &index->chunk_a;
	tab[1] = &index->chunk_b;
	tab[2] = &index->chunk_c;
	tab[3] = &index->chunk_d;
	sector = addr_to_sector(addr, index);
	required_sectors = get_required_sectors(size, index->type);
	mask = (((uint64_t)1 << required_sectors) - 1) << (sector & BLOC_MASK);
	*tab[sector >> BLOC_COUNT_SHR] &= ~mask;
	if (*tab[0] == 0 && *tab[1] == 0 && *tab[2] == 0 && *tab[3] == 0)
	{
		destroy_pages((void *)addr,
			(index->type == TINY) ? TINY_RANGE : MEDIUM_RANGE);
		return (destroy_index(index));
	}
	return (0);
}

/*
** Return an index_page and the value n of a specified index.
*/

struct s_index_page	*find_index_page(
	uint64_t addr,
	enum e_page_type page_type,
	int *i)
{
	struct s_index_page		*index_page;
	struct s_index			*index;
	uint64_t				off_last;

	*i = 0;
	index_page = ctx.last_index_page;
	off_last = (page_type == TINY) ? TINY_RANGE : MEDIUM_RANGE;
	index_page = ctx.last_index_page;
	while (index_page)
	{
		*i = index_page->primary_block.nb_index - 1;
		while (*i >= 0)
		{
			index = &index_page->index[*i];
			if (page_type == index->type &&
				addr >= index->page_addr &&
				addr < index->page_addr + off_last)
			{
				return (index_page);
			}
			(*i)--;
		}
		index_page = index_page->primary_block.prev;
	}
	return (NULL);
}

/*
** Delete association.
*/

int					del_index(uint64_t addr, size_t size)
{
	struct s_index_page		*index_page;
	enum e_page_type		page_type;
	int						i;

	page_type = (size <= TINY_LIMIT) ? TINY : MEDIUM;
	index_page = find_index_page(addr, page_type, &i);
	if (index_page)
		return (unreg_mark(&index_page->index[i], addr, size));
	return (-1);
}
