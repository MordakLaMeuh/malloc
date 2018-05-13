/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_custom_allocator.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

/*
** RESERVE NEW PAGES:
** Ask mmap for pages.
** Create an index node. (get index node)
** Create a record node.
** Associate index node to pages
** Associate record node to free
** Insert index
** Insert free record
*/

/*
** Node use when malloc:
** One free node converted in another free node = 0
** Can liberate one free tree ressource
** But can necesify on record tree ressource.
** Allocation of a new record
*/

/*
** MALLOC:
** Get free tree record
** Take free record
** Modifying free record
**
** Create a record node
** Get free record
** Modifying free tree record
** Cut free record
** Cut free tree record
*/

/*
** FREE:

**
** PRIORITY IS TO AVOID SUPPRESSION OF NODES
*/
