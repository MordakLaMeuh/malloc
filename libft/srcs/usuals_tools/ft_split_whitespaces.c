/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_whitespaces.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/17 16:10:12 by bmickael          #+#    #+#             */
/*   Updated: 2017/03/19 00:22:30 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char			**create_tab(char *s, int *n_words)
{
	char **tab;

	while (*s)
	{
		if (*s != ' ' && *s != '\t' && *s != '\n')
		{
			*n_words += 1;
			while (*s && *s != ' ' && *s != '\t' && *s != '\n')
				s++;
			while (*s && (*s == ' ' || *s == '\t' || *s == '\n'))
				s++;
		}
		else
			s++;
	}
	if (!(tab = (char **)malloc((*n_words + 1) * sizeof(char *))))
		return (NULL);
	tab[*n_words] = NULL;
	return (tab);
}

static void		ft_strncpy(char *dst, char *src, int n)
{
	while (n--)
		*dst++ = *src++;
}

char			**fill_tab(char *s, char **tab)
{
	char	*ptr;
	int		i;
	int		j;

	i = 0;
	while (*s)
	{
		if (*s != ' ' && *s != '\t' && *s != '\n')
		{
			ptr = s;
			while (*s && *s != ' ' && *s != '\t' && *s != '\n')
				s++;
			j = s - ptr;
			if (!(tab[i] = (char *)malloc((j + 1) * sizeof(char))))
				return (NULL);
			tab[i][j] = '\0';
			ft_strncpy(tab[i++], ptr, j);
			while (*s && (*s == ' ' || *s == '\t' || *s == '\n'))
				s++;
		}
		else
			s++;
	}
	return (tab);
}

char			**ft_split_whitespaces(char *str, int *len)
{
	char	**tab;
	int		n_words;

	n_words = 0;
	if (!(tab = create_tab(str, &n_words)))
		return (NULL);
	if (len)
		*len = n_words;
	return (fill_tab(str, tab));
}
