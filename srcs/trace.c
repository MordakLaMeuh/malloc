/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 17:55:27 by bmickael          #+#    #+#             */
/*   Updated: 2018/04/22 18:11:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main_headers.h"

void			open_malloc_tracer(void)
{
	char	*value;
	int		ret;

	value = getenv("MALLOC_TRACER");
	if (value == NULL)
	{
		ctx.tracer_file_descriptor = -1;
		return ;
	}
	ctx.tracer_file_descriptor = open(value, O_WRONLY);
	if (ctx.tracer_file_descriptor == -1)
		return ;
	ret = clock_gettime(CLOCK_MONOTONIC, &ctx.begin_time);
	if (ret == -1)
		ft_bzero(&ctx.begin_time, sizeof(struct timespec));
	ctx.begin_time.tv_nsec /= 1000;
}

static void		write_body(
		enum e_op_type op,
		void *ptr,
		size_t size_a,
		size_t size_b)
{
	if (op == MALLOC)
		ft_fprintf(ctx.tracer_file_descriptor, "{magenta}Malloc{eoc} (%lu) ",
				size_a);
	else if (op == FREE)
		ft_fprintf(ctx.tracer_file_descriptor, "{cyan}Free{eoc} (%p) ", ptr);
	else if (op == CALLOC)
		ft_fprintf(ctx.tracer_file_descriptor,
				"{green}Calloc{eoc} (%p, %lu, %lu) ",
				ptr, size_a, size_b);
	else if (op == REALLOC)
		ft_fprintf(ctx.tracer_file_descriptor,
				"{yellow}Realloc{eoc} (%p, %lu) ", ptr, size_a);
	else if (op == REALLOCF)
		ft_fprintf(ctx.tracer_file_descriptor,
				"{green}ReallocF{eoc} (%p, %lu) ", ptr, size_a);
	else if (op == REALLOCARRAY)
		ft_fprintf(ctx.tracer_file_descriptor,
				"{green}ReallocArray {eoc}(%p, %lu, %lu) ", ptr,
				size_a, size_b);
	else if (op == VALLOC)
		ft_fprintf(ctx.tracer_file_descriptor,
				"{magenta}Valloc{eoc} (%lu) ", ptr, size_a);
}

void			begin_trace(
		enum e_op_type op,
		void *ptr,
		size_t size_a,
		size_t size_b)
{
	struct timespec	now;
	int				ret;

	ret = clock_gettime(CLOCK_MONOTONIC, &now);
	if (ret == 0)
	{
		now.tv_nsec /= 1000;
		if (now.tv_nsec > ctx.begin_time.tv_nsec)
			now.tv_nsec = now.tv_nsec - ctx.begin_time.tv_nsec;
		else
		{
			now.tv_nsec = (now.tv_nsec + 1000000) - ctx.begin_time.tv_nsec;
			now.tv_sec -= 1;
		}
		now.tv_sec = now.tv_sec - ctx.begin_time.tv_sec;
		ft_fprintf(ctx.tracer_file_descriptor,
				"%.4lu.%.6lu ", now.tv_sec, now.tv_nsec);
	}
	write_body(op, ptr, size_a, size_b);
}

void			bend_trace(enum e_trace_result result)
{
	if (result == SUCCESS)
		ft_fprintf(ctx.tracer_file_descriptor, "{green}Success{eoc}\n");
	else if (result == FAIL)
		ft_fprintf(ctx.tracer_file_descriptor, "{red}Fail{eoc}\n");
	else
		ft_fprintf(ctx.tracer_file_descriptor, "{magenta}No action{eoc}\n");
}

void			close_malloc_tracer(void)
{
	if (ctx.tracer_file_descriptor < 0)
		return ;
	close(ctx.tracer_file_descriptor);
}
