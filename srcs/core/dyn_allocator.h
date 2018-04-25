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

/*
** Global description
*/

struct s_data_page;
struct s_record_page;
struct s_index_page;

struct s_ctx {
	size_t					page_size;
    struct rlimit			mem_limit;
    struct s_record_page	*first_record_page;
	struct s_record_page	*last_record_page;
	struct s_index_page		*first_index_page;
	struct s_index_page		*last_index_page;
	int						record_density;
	int						index_density;
} ctx;

/*
** Color for Black & White binary tree
*/

enum		e_color {
	RED = 0,
	BLACK
};

/*
** Data page
*/

# define TINY_BLOCK_SIZE	16
# define MEDIUM_BLOCK_SIZE	512
# define TINY_LIMIT			496
# define MEDIUM_LIMIT		16124

struct		s_data_page {
	void					*content;
};

/*
** Record
*/

struct s_record;

struct		s_primary_record {
	struct s_record_page	*next;
	struct s_record_page	*prev;
    int						nb_record;
} __attribute__((aligned(32)));

struct		s_record {
	void					*addr;
	struct s_record			*left_addr;
	struct s_record			*right_addr;
	enum e_color			color_addr;
	uint32_t				size;
} __attribute__((aligned(32)));

struct		s_record_page {
	struct s_primary_record	primary_block;
	struct s_record			record[];
};

/*
** Index
*/

struct s_index;

struct		s_primary_index {
	struct s_index_page		*next;
	struct s_index_page		*prev;
    int						nb_index;
} __attribute__((aligned(128)));

enum		e_page_type {
	TINY = 0,
	MEDIUM,
	LARGE
};

struct		s_index {
	__uint64_t				chunk_a;
	__uint64_t				chunk_b;
	__uint64_t				chunk_c;
	__uint64_t				chunk_d;
	struct s_index			*left_addr;
	struct s_index			*right_addr;
	enum e_color			color_addr;
	struct s_index			*left_space;
	struct s_index			*right_space;
	enum e_color			color_space;
	struct s_data_page		*page;
	enum e_page_type		type;
} __attribute__((aligned(128)));

struct		s_index_page {
	struct s_primary_index	primary_block;
	struct s_index			index[];
};

/*
** Functions
*/

void __attribute__((constructor))	_constructor();
void __attribute__((destructor))	_destructor();

void								*get_new_pages(int nb);
int									destroy_pages(void *addr, int nb);

struct s_record						*search_record(void *addr);
struct s_record						*get_new_record(void);
int									del_record(struct s_record *record);

#endif
