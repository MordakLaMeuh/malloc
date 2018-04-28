/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

static uint64_t				debug_record(uint64_t *q)
{
	struct s_record_page	*record_page;
	struct s_record			*record;
	int						i;
	int						page_count;
	int						record_count;

	i = 0;
	page_count = 0;
	record_count = 0;
	record_page = ctx.last_record_page;
	while (record_page)
	{
		page_count++;
		i = 0;
		while (i < record_page->primary_block.nb_record)
		{
			record = &record_page->record[i++];
			*q += (record->size > MEDIUM_LIMIT) ? record->size : 0;
		}
		record_count += record_page->primary_block.nb_record;
		record_page = record_page->primary_block.prev;
	}
	ft_printf("Got %i record page for %i record\n", page_count, record_count);
	return (ctx.page_size * page_count);
}

static uint64_t				debug_index(uint64_t *q)
{
	struct s_index_page		*index_page;
	struct s_index			*index;
	int						i;
	int						page_count;
	int						index_count;

	i = 0;
	page_count = 0;
	index_count = 0;
	index_page = ctx.last_index_page;
	while (index_page)
	{
		page_count++;
		i = 0;
		while (i < index_page->primary_block.nb_index)
		{
			index = &index_page->index[i++];
			*q += (index->type == TINY) ? TINY_RANGE : MEDIUM_RANGE;
		}
		index_count += index_page->primary_block.nb_index;
		index_page = index_page->primary_block.prev;
	}
	ft_printf("Got %i index page for %i index\n", page_count, index_count);
	return (ctx.page_size * page_count);
}

static struct s_index			**got_index_tab(size_t *len)
{
	struct s_index_page			*index_page;
	struct s_index				**t1;
	int							i;

	*len = 0;
	index_page = ctx.last_index_page;
	while (index_page)
	{
		*len += index_page->primary_block.nb_index;
		index_page = index_page->primary_block.prev;
	}
	index_page = ctx.last_index_page;
	if ((t1 = (struct s_index **)get_new_pages(*len * sizeof(struct s_index *))) == NULL)
		return (NULL);
	*len = 0;
	while (index_page)
	{
		i = -1;
		while (++i < index_page->primary_block.nb_index)
			t1[(*len)++] = &index_page->index[i];
		index_page = index_page->primary_block.prev;
	}
	return (t1);
}

static struct s_record			**got_record_tab(size_t *len)
{
	struct s_record_page		*record_page;
	struct s_record				**t1;
	int							i;

	*len = 0;
	record_page = ctx.last_record_page;
	while (record_page)
	{
		*len += record_page->primary_block.nb_record;
		record_page = record_page->primary_block.prev;
	}
	record_page = ctx.last_record_page;
	if ((t1 = (struct s_record **)get_new_pages(*len * sizeof(struct s_record *))) == NULL)
		return (NULL);
	*len = 0;
	while (record_page)
	{
		i = -1;
		while (++i < record_page->primary_block.nb_record)
			t1[(*len)++] = &record_page->record[i];
		record_page = record_page->primary_block.prev;
	}
	return (t1);
}

static int					page_addr_comp(void *i1, void *i2)
{
	if (((struct s_index *)i1)->page_addr < ((struct s_index *)i2)->page_addr)
		return (0);
	return (1);
}

static int					record_addr_comp(void *i1, void *i2)
{
	if (((struct s_record *)i1)->addr < ((struct s_record *)i2)->addr)
		return (0);
	return (1);
}

void						display_debug(
		struct s_index **index_tab,
		struct s_record **record_tab,
		size_t len_i,
		size_t len_r)
{
	size_t c_i;
	size_t c_r;
	uint64_t addr;

	c_i = -1;
	c_r = 0;
	while (++c_i < len_i)
	{
		if (index_tab[c_i]->type == TINY)
			ft_printf("TINY : %p\n", index_tab[c_i]->page_addr);
		else
			ft_printf("%s : %p\n", index_tab[c_i]->type == MEDIUM ?
					"SMALL": "LARGE", index_tab[c_i]->page_addr);
		if (c_i + 1 != len_i)
			addr = index_tab[c_i + 1]->page_addr;
		else
			addr = 0xffffffffffffffff;
		while (c_r < len_r && record_tab[c_r]->addr < addr)
		{
			ft_printf("%p - %p : %lu octets\n", record_tab[c_r]->addr,
				record_tab[c_r]->addr + record_tab[c_r]->size,
				record_tab[c_r]->size);
			c_r++;
		}
	}
}

static int					debug_tab(void)
{
	struct s_index **index_tab;
	struct s_record **record_tab;
	size_t len_i;
	size_t len_r;
	int ret;

	ret = -1;
	if ((index_tab = got_index_tab(&len_i)) == NULL)
		return (ret);
	if ((record_tab = got_record_tab(&len_r)) == NULL)
	{
		destroy_pages(index_tab, len_i * sizeof(struct s_index **));
		return (ret);
	}
	if ((ft_merge_tab_malloc(
			(void ***)&index_tab, len_i, &page_addr_comp) != -1) &&
	ft_merge_tab_malloc(
			(void ***)&record_tab, len_r, &record_addr_comp) != -1)
	{
		display_debug(index_tab, record_tab, len_i, len_r);
		ret = 0;
	}
	destroy_pages(index_tab, len_i * sizeof(struct s_index **));
	destroy_pages(record_tab, len_r * sizeof(struct s_record **));
	return (ret);
}

void						core_debug(void)
{
	uint64_t total_occupied_size;
	uint64_t record_size;
	uint64_t index_size;

	debug_tab();

	total_occupied_size = 0;
	record_size = debug_record(&total_occupied_size);
	index_size = debug_index(&total_occupied_size);
	ft_printf("Total occupation:\n-> Record: %lu\n-> Index: %lu\n",
		record_size,
		index_size);
	ft_printf("-> Allocated for programm: %lu\nTOTAL: %lu\n",
		total_occupied_size,
		total_occupied_size + record_size + index_size);
}
