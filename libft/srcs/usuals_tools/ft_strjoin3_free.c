/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin3_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/10 06:54:29 by bmickael          #+#    #+#             */
/*   Updated: 2017/09/10 06:54:34 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

static void		strdelete(char *s1, char *s2, char *s3, int delete)
{
	if (delete & 0x1)
		free(s3);
	if (delete & 0x2)
		free(s2);
	if (delete & 0x4)
		free(s1);
}

static void		copy(char *s1, char *s2, size_t index_min, size_t index_max)
{
	size_t i;

	i = 0;
	while (i < index_min)
	{
		s1[index_max + i] = s2[i];
		i++;
	}
}

char			*ft_strjoin3_free(char *s1, char *s2, char *s3, int del)
{
	char		*r;
	size_t		n1;
	size_t		n2;
	size_t		n3;

	n1 = ft_strlen(s1);
	n2 = ft_strlen(s2);
	n3 = ft_strlen(s3);
	if (!(s1) || !(s2) || !(s3))
		return (NULL);
	if (!(r = ft_strnew(n1 + n2 + n3)))
		return (NULL);
	copy(r, s1, n1, 0);
	copy(r, s2, n2, n1);
	copy(r, s3, n3, n1 + n2);
	strdelete(s1, s2, s3, del);
	return (r);
}
