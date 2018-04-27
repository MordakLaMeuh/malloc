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

void						core_debug(void)
{
	uint64_t total_occupied_size;
	uint64_t record_size;
	uint64_t index_size;

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
