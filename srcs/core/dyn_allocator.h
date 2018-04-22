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

# define TINY_SIZE_MAX 64
# define MEDIUM_SIZE_MAX 4096

# include <sys/mman.h>
# include <stdint.h>

/*
** XXX Debuging include
*/
# include <stdio.h>

# include <libft.h>

struct s_idx_page;

enum		e_page_type {
	TINY = 0,
	MEDIUM,
	LARGE
};

enum		e_allocated {
	NO_ALLOCATED = 0,
	ALLOCATED
};

struct					s_ctx {
	size_t				page_size;
	int					idx_page_count;
	struct s_idx_page	*first_idx_page;
} ctx;

struct		s_primary_block {
	struct s_idx_page	*next_idx_page;
	int					nb_page_desc;
} __attribute__((aligned(32)));

struct		s_idx_page_description {
	uint64_t			mask_sector_a;
	uint64_t			mask_sector_b;
	uint64_t			mask_sector_c;
	uint64_t			mask_sector_d;
	enum e_page_type	type;
	void				*page;
	enum e_allocated	allocated;
} __attribute__((aligned(64)));

struct		s_idx_page {
	struct s_primary_block			primary_block;
	struct s_idx_page_description	page_desc_field[];
};

void		before_start(void) __attribute__((constructor));

struct s_idx_page				*get_new_idx_page(void);

void							*claim_new_data_page(void);
struct s_idx_page_description	*create_new_idx(
	struct s_idx_page *idx_page,
	int n_idx,
	enum e_page_type type);


#endif
