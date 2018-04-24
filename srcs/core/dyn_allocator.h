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

//# define DEBUG_INFO 0

# include <sys/mman.h>
# include <stdint.h>
# include <sys/resource.h>

/*
** XXX Debuging include
*/
# include <stdio.h>

# include <libft.h>

struct s_idx_page;
struct s_record_page;
struct s_data_page;

struct		s_ctx {
	size_t					page_size;
    struct rlimit			mem_limit;
    struct s_record_page	*first_record_page;
	struct s_record_page	*last_record_page;
	int						record_density;
} ctx;

struct		s_primary_record {
	struct s_record_page   *next;
	struct s_record_page   *prev;
    int                     nb_record;
} __attribute__((aligned(32)));

struct		s_record {
	void					*addr;
	size_t					size;
} __attribute__((aligned(16)));

struct		s_record_page {
	struct s_primary_record	primary_block;
	struct s_record			record[];
};

void __attribute__((constructor))	_constructor();
void __attribute__((destructor))	_destructor();

void								*get_new_pages(int nb);
int									destroy_pages(void *addr, int nb);

struct s_record						*search_record(void *addr);
struct s_record						*get_new_record(void);
int									del_record(struct s_record *record);





# define TINY_SIZE_MAX 1024 - 16
# define MEDIUM_SIZE_MAX 131072 - 512

enum		e_page_type {
	TINY = 0,
	MEDIUM,
	LARGE
};

struct		s_data_page {
	uint8_t				*content;
};

struct		s_primary_idx_block {
	struct s_idx_page	*next;
	int					nb_idx;
} __attribute__((aligned(64)));

struct		s_idx_page_description {
	__uint128_t			mask_sector_a;
	__uint128_t			mask_sector_b;
	enum e_page_type	type;
	struct s_data_page	*page;
} __attribute__((aligned(64)));

struct		s_idx_page {
	struct s_primary_idx_block		primary_block;
	struct s_idx_page_description	page_desc_field[];
};

#endif
