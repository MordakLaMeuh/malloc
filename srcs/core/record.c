/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   record.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

struct s_record	*get_new_record(void)
{
	struct s_record_page	*tmp;
	int						*nb_record;

	tmp = ctx.last_record_page;
	if (tmp == NULL || tmp->primary_block.nb_record == ctx.record_density)
	{
		ctx.last_record_page = (struct s_record_page *)get_new_pages(1);
		if (ctx.last_record_page == NULL)
			return NULL;
		if (ctx.first_record_page == NULL)
			ctx.first_record_page = ctx.last_record_page;
		nb_record = &ctx.last_record_page->primary_block.nb_record;
		ctx.last_record_page->primary_block.next = NULL;
		ctx.last_record_page->primary_block.prev = tmp;
		if (tmp != NULL)
			tmp->primary_block.next = ctx.last_record_page;
		*nb_record = 1;
	}
	else
	{
		nb_record = &ctx.last_record_page->primary_block.nb_record;
		*nb_record += 1;
	}
	return &ctx.last_record_page->record[*nb_record - 1];
}

int				del_record(struct s_record *record)
{
	struct s_record_page	*last_record_page;
	struct s_record			*last_record;

	last_record_page = ctx.last_record_page;
	if (last_record_page == NULL)
		return -1;
	last_record = &last_record_page->record
		[last_record_page->primary_block.nb_record - 1];
	last_record_page->primary_block.nb_record -= 1;
	if (record != last_record)
		ft_aligned_memcpy(record, last_record, sizeof(struct s_record));
	if (last_record_page->primary_block.nb_record == 0)
	{
		if (last_record_page->primary_block.prev)
			last_record_page->primary_block.prev->primary_block.next = NULL;
		else
			ctx.first_record_page = NULL;
		ctx.last_record_page = last_record_page->primary_block.prev;
		if (destroy_pages(last_record_page, 1) < 0)
			return -1;
	}
	return 0;
}

struct s_record	*search_record(void *addr)
{
	struct s_record_page	*record_page;
	int						i;

	record_page = ctx.last_record_page;
	while (record_page != NULL)
	{
		i = record_page->primary_block.nb_record - 1;
		while (i != -1)
		{
			if (record_page->record[i].addr == addr)
				return &record_page->record[i];
			i--;
		}
		record_page = record_page->primary_block.prev;
	}
	return NULL;
}
