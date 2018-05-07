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

#ifndef MAIN_HEADERS_H
# define MAIN_HEADERS_H

# include <unistd.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <stdint.h>
# include <sys/resource.h>
# include <pthread.h>
# include <stdbool.h>

# include "libft.h"
# include "btree.h"

/*
** XXX Debug
*/

# include <assert.h>

/*
** Data page
*/

# define RECORD_ALLIGN		16
# define RECORD_REQ_PAGES	4

# define INDEX_ALLIGN		64
# define INDEX_REQ_PAGES	2

# define NODE_ALLIGN		32
# define NODE_REQ_PAGES		8

# define TINY_SHR			4
# define TINY_MASK			0xf
# define TINY_BLOCK_SIZE	(1 << TINY_SHR)
# define TINY_LIMIT			(TINY_BLOCK_SIZE * 32 - TINY_BLOCK_SIZE)
# define TINY_RANGE			(TINY_LIMIT * 256)

# define MEDIUM_SHR			9
# define MEDIUM_MASK		0x1ff
# define MEDIUM_BLOCK_SIZE	(1 << MEDIUM_SHR)
# define MEDIUM_LIMIT		(MEDIUM_BLOCK_SIZE * 32 - MEDIUM_BLOCK_SIZE)
# define MEDIUM_RANGE		(MEDIUM_LIMIT * 256)

/*
** Global description
*/

enum		e_page_type {
	TINY = 0,
	MEDIUM,
	LARGE
};

struct s_data_page;
struct s_record_page;
struct s_index_page;
struct s_node_page;

struct								s_ctx {
	size_t					page_size;
	struct rlimit			mem_limit;

	struct s_record_page	*record_pages_entry;
	struct s_node_page		*node_pages_entry;
	struct s_index_page		*index_pages_entry;
	struct s_node			*tiny_index_pages_tree;
	struct s_node			*medium_index_pages_tree;
	struct s_node			*big_page_record_tree;

	int						record_density;
	int						node_density;
	int						index_density;

	bool					is_initialized;
} ctx;

struct								s_data_page {
	uint64_t				*content;
};

/*
** Record Pages Structure
*/

struct s_record;

struct								s_primary_record {
	struct s_record_page	*next;
	int						nb_record;
} __attribute__((aligned(RECORD_ALLIGN)));

struct								s_record {
	uint64_t				addr;
	size_t					size;
} __attribute__((aligned(RECORD_ALLIGN)));

struct								s_record_page {
	struct s_primary_record	primary_block;
	struct s_record			record[];
};

/*
** Index Pages Structure
*/

struct s_index;

struct								s_primary_index {
	struct s_index_page		*next;
	int						nb_index;
} __attribute__((aligned(INDEX_ALLIGN)));

struct								s_index {
	struct s_node			*allocation_tree;
	struct s_node			*space_tree;
	__uint64_t				page_addr;
	enum e_page_type		type;
} __attribute__((aligned(INDEX_ALLIGN)));

struct								s_index_page {
	struct s_primary_index	primary_block;
	struct s_index			index[];
};

/*
** Node Pages Structure
*/

struct								s_primary_node {
	struct s_node_page		*next;
	int						nb_node;
} __attribute__((aligned(NODE_ALLIGN)));

struct								s_node_page {
	struct s_primary_node	primary_block;
	struct s_node			*node;
};

/*
** Main Functions
*/

void								*core_allocator(size_t *size);
void								core_deallocator(void *addr);
void								*core_realloc(void *ptr, size_t size);

/*
** Mem_syscall functions
*/

void								*get_new_pages(size_t size);
int									destroy_pages(void *addr, size_t size);

/*
** Pages management
*/

int									assign_new_chunk(
	void *addr,
	enum e_page_type page_type);

/*
** Custom memory management
*/

void								*node_custom_allocator(size_t size);
struct s_index						*index_custom_allocator(void);
struct s_record						*record_custom_allocator(void);

void								node_custom_deallocator(void *node);
void								index_custom_deallocator(
	struct s_index *index);
void								record_custom_deallocator(
	struct s_record *record);

int									cmp_index_addr(void *i1, void *i2);
int									cmp_record_size(void *r1, void *r2);
int									cmp_record_addr(void *r1, void *r2);
int									cmp_tiny_addr_range(void *r1, void *r2);
int									cmp_medium_addr_range(void *r1, void *r2);

/*
** Finders.
*/

struct s_node						*get_associated_node_for_record(
	struct s_record *record);
struct s_node						*get_associated_node_for_index(
	struct s_index *index);

struct s_node						*search_record_node(
	uint64_t addr,
	struct s_index **index);

/*
** Size tools.
*/

size_t								allign_size(
	size_t size,
	enum e_page_type page_type);

enum e_page_type					get_page_type(size_t size);

#endif
