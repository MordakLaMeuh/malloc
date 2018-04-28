/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctor.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:47:28 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:14:49 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CTOR_H
# define CTOR_H

void __attribute__((constructor))	main_constructor();
void __attribute__((destructor))	main_destructor();

#endif
