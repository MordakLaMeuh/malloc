/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dyn_alocator.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DYN_ALLOCATOR_H
# define DYN_ALLOCATOR_H

# define TINY_SIZE_MAX 1024 - 16
# define MEDIUM_SIZE_MAX 131072 - 512

# include <sys/mman.h>
# include <stdint.h>

/*
** XXX Debuging include
*/
# include <stdio.h>

# include <libft.h>

struct s_idx_page;
struct s_reg_page;

struct		s_ctx {
	size_t				page_size;
	struct s_idx_page	*first_idx_page;
	struct s_reg_page	*first_reg_page;
} ctx;

enum		e_page_type {
	TINY = 0,
	MEDIUM,
	LARGE
};

enum		e_allocated {
	NO_ALLOCATED = 0,
	ALLOCATED
};

struct		s_data_page {
	uint8_t				*content;
};

struct		s_primary_idx_block {
	struct s_idx_page	*next;
	int					nb_idx;
} __attribute__((aligned(64)));

struct		s_primary_reg_block {
	struct s_reg_page	*next;
	size_t				nb_reg;
} __attribute__((aligned(16)));

struct		s_idx_page_description {
	uint64_t			mask_sector_a;
	uint64_t			mask_sector_b;
	uint64_t			mask_sector_c;
	uint64_t			mask_sector_d;
	enum e_page_type	type;
	struct s_data_page	*page;
	enum e_allocated	allocated;
} __attribute__((aligned(64)));

struct		s_reg {
	void	*addr;
	size_t	size;
} __attribute__((aligned(16)));

struct		s_reg_page {
	struct s_primary_reg_block		primary_block;
	struct s_reg					reg[];
};

struct		s_idx_page {
	struct s_primary_idx_block		primary_block;
	struct s_idx_page_description	page_desc_field[];
};

void		before_start(void) __attribute__((constructor));

void		*get_new_pages(int nb);
int			destroy_pages(void *addr, int nb);

/*
** struct s_idx_page_description	*create_new_idx(
**	struct s_idx_page *idx_page,
**	int n_idx,
**	enum e_page_type type);
*/

#endif
