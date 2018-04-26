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

// # define DEBUG_CTOR 0
// # define MAIN_DEBUG 0

# include <unistd.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <stdint.h>
# include <sys/resource.h>
# include <pthread.h>

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
struct s_node_page;

struct s_ctx {
	size_t					page_size;
    struct rlimit			mem_limit;
    struct s_record_page	*first_record_page;
	struct s_record_page	*last_record_page;
	struct s_index_page		*first_index_page;
	struct s_index_page		*last_index_page;
	int						record_density;
	int						index_density;
	struct s_node_page		*record_addr_tree;
	struct s_node_page		*index_addr_tree;
	struct s_node_page		*index_space_tree;
} ctx;

/*
** Data page
*/

# define TINY_SHR			4
# define MEDIUM_SHR			9
# define TINY_BLOCK_SIZE	(1 << TINY_SHR)
# define MEDIUM_BLOCK_SIZE	(1 << MEDIUM_SHR)
# define TINY_LIMIT			(TINY_BLOCK_SIZE * 32 - TINY_BLOCK_SIZE)
# define MEDIUM_LIMIT		(MEDIUM_BLOCK_SIZE * 32 - MEDIUM_BLOCK_SIZE)
# define TINY_RANGE			(TINY_BLOCK_SIZE * 256)
# define MEDIUM_RANGE		(MEDIUM_BLOCK_SIZE * 256)

# define RECORD_ALLIGN		16
# define INDEX_ALLIGN		64
# define NODE_ALLIGN		32

# define BLOC_MASK			64

struct		s_data_page {
	uint64_t				*content;
};

/*
** Record
*/

struct s_record;

struct		s_primary_record {
	struct s_record_page	*prev;
    int						nb_record;
} __attribute__((aligned(RECORD_ALLIGN)));

struct		s_record {
	uint64_t				addr;
	size_t					size;
} __attribute__((aligned(RECORD_ALLIGN)));

struct		s_record_page {
	struct s_primary_record	primary_block;
	struct s_record			record[];
};

/*
** Index
*/

struct s_index;

struct		s_primary_index {
	struct s_index_page		*prev;
    int						nb_index;
} __attribute__((aligned(INDEX_ALLIGN)));

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
	__uint64_t				page_addr;
	enum e_page_type		type;
} __attribute__((aligned(INDEX_ALLIGN)));

struct		s_index_page {
	struct s_primary_index	primary_block;
	struct s_index			index[];
};

/*
** Color for Black & White binary tree
*/

struct s_node;

enum		e_color {
	RED = 0,
	BLACK
};

struct		s_primary_node {
	struct s_node_page		*next;
	struct s_node_page		*prev;
    int						nb_node;
} __attribute__((aligned(NODE_ALLIGN)));

struct		s_node {
	struct s_node			*left;
	struct s_node			*right;
	void					*content;
	enum e_color			color;
} __attribute__((aligned(NODE_ALLIGN)));

struct		s_node_page {
	struct s_primary_node	primary_block;
	struct s_node			node[];
};

/*
** Functions
*/

void __attribute__((constructor))	_constructor();
void __attribute__((destructor))	_destructor();

void								*core_allocator(size_t size);

void								*get_new_pages(size_t size);
int									destroy_pages(void *addr, size_t size);

struct s_record						*search_record(uint64_t addr);
struct s_record						*get_new_record();
int									del_record(struct s_record *record);

uint64_t							assign_index(size_t size);
int									del_index(uint64_t addr, size_t size);

uint32_t							get_required_sectors(
	size_t size,
	enum e_page_type page_type);
uint64_t							sector_to_addr(
	uint64_t base_addr,
	enum e_page_type page_type,
	uint32_t sector);
uint32_t							addr_to_sector(
	uint64_t addr,
	struct s_index *index);
int									fill(
	uint64_t *field,
	uint32_t required_sectors);

void								core_debug(void);

#endif
