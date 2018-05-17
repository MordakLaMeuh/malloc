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
# include <errno.h>

# include "libft.h"
# include "btree.h"

/*
** Data page
*/

# define NODE_ALLIGN		64
# define NODE_REQ_PAGES		4

# define TINY_SHR			4
# define TINY_MAX_BLOCK		128

# define TINY_BLOCK_SIZE	(1 << TINY_SHR)
# define TINY_MASK			(TINY_BLOCK_SIZE - 1)
# define TINY_LIMIT			(TINY_BLOCK_SIZE * 32 - TINY_BLOCK_SIZE)
# define TINY_RANGE			(TINY_BLOCK_SIZE * 32 * TINY_MAX_BLOCK)

# define MEDIUM_SHR			9
# define MEDIUM_MAX_BLOCK	128

# define MEDIUM_BLOCK_SIZE	(1 << MEDIUM_SHR)
# define MEDIUM_MASK		(MEDIUM_BLOCK_SIZE - 1)
# define MEDIUM_LIMIT		(MEDIUM_BLOCK_SIZE * 32 - MEDIUM_BLOCK_SIZE)
# define MEDIUM_RANGE		(MEDIUM_BLOCK_SIZE * 32 * MEDIUM_MAX_BLOCK)

/*
** Global description
*/

struct s_node_page;

struct				s_ctx {
	size_t					page_size;
	struct rlimit			mem_limit;

	struct s_node_page		*node_pages_entry;
	struct s_node			*index_pages_tree;
	struct s_node			*global_tiny_space_tree;
	struct s_node			*global_medium_space_tree;
	struct s_node			*big_page_record_tree;

	int						node_density;
	struct s_node_page		*node_cache;

	bool					is_initialized;
}					ctx;

/*
** Node Pages Structure
*/

struct				s_primary_node {
	struct s_node_page		*next;
	int						nb_node;
} __attribute__((aligned(NODE_ALLIGN)));

struct				s_node_page {
	struct s_primary_node	primary_block;
	struct s_node			node[];
};

struct				s_couple {
	size_t					len;
	void					*addr;
};

/*
** Mem_syscall functions
*/

void				*get_new_pages(size_t size);
int					destroy_pages(void *addr, size_t size);

/*
** Main Functions
*/

void				*core_allocator(size_t *size);
void				core_deallocator(void *ptr);
void				*core_realloc(void *ptr, size_t *size, bool *memfail);

/*
** Special allocator
*/

void				*node_custom_allocator(size_t size);
void				node_custom_deallocator(void *node);

/*
** Free pages management
*/

int					insert_free_record(
		void *addr,
		size_t size,
		enum e_page_type type,
		struct s_node **parent_ref);

int					delete_free_record(
		struct s_node *record,
		struct s_node *parent,
		enum e_page_type type);

struct s_node		*get_free_record(
		void *addr,
		size_t size,
		struct s_node **parent,
		enum e_page_type type);

struct s_node		*get_best_free_record_tree(
		size_t size,
		enum e_page_type type);

/*
** Index management
*/

void				*insert_allocated_record(struct s_node *record);

void				**find_index_node(void *addr);

void				*create_index(
		void *addr,
		uint32_t range);

void				destroy_index(struct s_node *index);

/*
** Finders.
*/

int					cmp_addr_to_node_addr(
		void *addr,
		struct s_node *node_b);

int					cmp_node_addr_to_node_addr(
		struct s_node *node_a,
		struct s_node *node_b);

int					cmp_size_to_node_size(
		void *size,
		struct s_node *node_b);

int					cmp_node_size_to_node_size(
		struct s_node *node_a,
		struct s_node *node_b);

int					cmp_addr_to_node_m_addr_range(
		void *content,
		struct s_node *node);

int					cmp_node_m_addr_to_node_m_addr(
		struct s_node *node_a,
		struct s_node *node_b);

int					cmp_m_addr_to_node_m_addr(
		void *addr,
		struct s_node *node_b);

/*
** Size tools.
*/

size_t				allign_size(size_t size, enum e_page_type page_type);

enum e_page_type	get_page_type(size_t size);

/*
** Debug tools
*/

void				show_alloc(void);

void				debug_nodes(void);

void				show_alloc_mem(void);

void				*core_allocator_large(size_t *size);

/*
** deallocator_next content
*/

void				fflush_neighbours(
		size_t len,
		void *address,
		enum e_page_type type);

void				do_prev_job(
		struct s_couple *out,
		struct s_couple *s,
		struct s_node *record,
		struct s_node *index);

/*
** free_record_next content
*/

void				assign_parent_free_tiny(
		void *content,
		struct s_node *node);

void				assign_parent_free_medium(
		void *content,
		struct s_node *node);

int					check_index_destroy(
		void *addr,
		size_t size,
		enum e_page_type type);

#endif
