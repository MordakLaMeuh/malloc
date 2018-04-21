/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_null_term_tab_free.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/17 00:44:03 by bmickael          #+#    #+#             */
/*   Updated: 2017/09/17 00:44:20 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int		ft_null_term_tab_free(char ***group)
{
	int	i;

	if (!(*group))
		return (-1);
	i = 0;
	while ((*group)[i])
		free((*group)[i++]);
	free(*group);
	*group = NULL;
	return (0);
}
