/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sodo.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SODO_H
# define SODO_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdint.h>
# include <stdio.h>

void					*malloc(size_t size);
void					free(void *ptr);
void					show_alloc_mem(void);
void					show_alloc_mem_ex(void);
void					*realloc(void *ptr, size_t size);

# define TEST_LENGTH	100000
# define MAX_ALLOC		6000
# define NB_TESTS		10000000

struct					s_test {
	void				*ptr;
	uint8_t				c;
	size_t				size;
};

void					add_sodo(
		struct s_test tab_ptr[TEST_LENGTH],
		int nb_elmt);

void					del_sodo(
		struct s_test tab_ptr[TEST_LENGTH],
		int nb_elmt);

void					real_sodo(
		struct s_test tab_ptr[TEST_LENGTH],
		int *nb_elmt);

uint64_t				get_timestamp(void);

#endif
