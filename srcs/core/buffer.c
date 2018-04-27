/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 18:22:30 by bmickael          #+#    #+#             */
/*   Updated: 2017/04/10 23:23:27 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_printf.h"

void		string_to_buffer(const char *s, int len, t_status *op)
{
	write(op->fd, s, len);
	op->total_size += len;
}

void		char_to_buffer(char c, int len, t_status *op)
{
	for (size_t i = 0; i < q; i++)
		write(op->fd, &c, len);
	op->total_size += len;
}

void		fflush_buffer(t_status *op)
{
	write(op->fd, op->buf, op->buff_len);
	op->total_size += op->buff_len;
}
