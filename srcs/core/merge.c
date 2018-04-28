/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:44:07 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 17:50:30 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dyn_allocator.h"

static void		merge_mod(void **s1, void **s2, void **end, t_info *w)
{
	void **p_gr_1;
	void **p_gr_2;

	while ((p_gr_1 = s1) < end)
	{
		p_gr_2 = p_gr_1 + w->offset;
		while (TRUE)
		{
			if (p_gr_2 < end)
				*s2++ = w->cmp(*p_gr_2, *p_gr_1) ? *p_gr_1++ : *p_gr_2++;
			if (p_gr_1 == (s1 + w->offset))
			{
				while (p_gr_2 != (s1 + (2 * w->offset)) && p_gr_2 < end)
					*s2++ = *p_gr_2++;
				break ;
			}
			else if (p_gr_2 == (s1 + (2 * w->offset)) || p_gr_2 >= end)
			{
				while (p_gr_1 != (s1 + w->offset) && p_gr_1 < end)
					*s2++ = *p_gr_1++;
				break ;
			}
		}
		s1 += 2 * w->offset;
	}
}

static void		**exec(void **t1, void **t2, int l, int (*cmp)(void *, void *))
{
	t_info		w;
	int			state;

	ft_bzero(&w, sizeof(t_info));
	w.cmp = cmp;
	w.offset = 1;
	state = FALSE;
	while (w.offset < l)
	{
		if (state == FALSE)
			merge_mod(t1, t2, t1 + l, &w);
		else
			merge_mod(t2, t1, t2 + l, &w);
		state = (state) ? FALSE : TRUE;
		w.offset <<= 1;
	}
	return ((state) ? t2 : t1);
}

int				ft_merge_tab_malloc(void ***t1, int len,
	int (*cmp)(void *, void *))
{
	void **t2;
	void **tmp;

	if (!len)
		return (0);
	t2 = (void **)get_new_pages(len * sizeof(void *));
	if (t2 == NULL)
		return (-1);
	if ((tmp = exec(*t1, t2, len, cmp)) == *t1)
		destroy_pages(t2, len * sizeof(void *));
	else
	{
		destroy_pages(t1, len * sizeof(void *));
		*t1 = tmp;
	}
	return (len);
}
