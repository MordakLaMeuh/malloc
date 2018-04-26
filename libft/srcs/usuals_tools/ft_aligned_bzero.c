/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aligned_bzero.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/10 14:27:36 by bmickael          #+#    #+#             */
/*   Updated: 2017/04/26 01:30:19 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	ft_aligned_bzero(void *s, size_t n)
{
	uint64_t *dst;
	uint8_t *e;

	dst = (uint64_t *)s;
	while (n >= 8)
	{
		*dst++ = (uint64_t)0;
		n -= 8;
	}
	e = (uint8_t *)dst;
	while (n)
	{
		*e++ = (uint8_t)0;
		n -= 1;
	}
}
