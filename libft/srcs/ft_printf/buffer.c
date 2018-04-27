/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 18:00:39 by bmickael          #+#    #+#             */
/*   Updated: 2017/04/10 18:02:28 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_printf.h"

void	fflush_buffer(t_status *op)
{
	if (!op->str)
	{
		write(op->fd, op->buf, op->buff_len);
	}
	else
	{
		ft_memcpy(op->str, op->buf, op->buff_len);
		op->str += op->buff_len;
	}
	op->total_size += op->buff_len;
	op->buff_len = 0;
}

void	string_to_buffer(const char *s, int len, t_status *op)
{
	int i;

	while (len > (MAX_BUF_LEN - op->buff_len))
	{
		i = MAX_BUF_LEN - op->buff_len;
		ft_memcpy(op->buf + op->buff_len, s, i);
		s += i;
		op->buff_len += i;
		fflush_buffer(op);
		len -= i;
	}
	ft_memcpy(op->buf + op->buff_len, s, len);
	op->buff_len += len;
}

void	char_to_buffer(char c, int len, t_status *op)
{
	int i;

	while (len > (MAX_BUF_LEN - op->buff_len))
	{
		i = MAX_BUF_LEN - op->buff_len;
		ft_memset(op->buf + op->buff_len, c, i);
		op->buff_len += i;
		fflush_buffer(op);
		len -= i;
	}
	ft_memset(op->buf + op->buff_len, c, len);
	op->buff_len += len;
}
