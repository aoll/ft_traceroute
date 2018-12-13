/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 00:49:02 by alex              #+#    #+#             */
/*   Updated: 2018/12/12 23:43:42 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void		print_by_unit(int time_echo)
{
	double	tf;
	int		t;
	double	f;

	tf = time_echo * 0.1;
	t = time_echo / 10;
	f = t * 1.0;
	if (f != tf)
	{
		printf(" time=%0.1f ms\n", tf);
	}
	else
	{
		printf(" time=%d ms\n", t);
	}
}

void			display(
	struct icmphdr *hdr2, struct timeval *t_time2, struct ip *iph2, t_env *e)
{
	int				time_echo;
	double			time_echo_u;
	struct timeval	t_time;

	gettimeofday(&t_time, NULL);
	time_echo = ((1000000 * t_time.tv_sec + t_time.tv_usec)
		- (1000000 * (t_time2->tv_sec) + t_time2->tv_usec)) / 100;
	time_echo_u = ((1000000 * t_time.tv_sec + t_time.tv_usec)
		- (1000000 * (t_time2->tv_sec) + t_time2->tv_usec));
	if (!time_echo || !time_echo_u)
	{
		printf("ft_ping: sendto: Network is unreachable\n");
		return ;
	}
	printf("%lu bytes from %s: icmp_seq=%d ttl=%u", PACKETSIZE_TOTAL -
		sizeof(struct ip), e->ipv4, hdr2->un.echo.sequence, iph2->ip_ttl);
	print_by_unit(time_echo);
	time_echo_u /= 1000;
	e->total += time_echo_u;
	e->tsum += time_echo_u * time_echo_u;
	e->min = e->min == 0 || time_echo_u < e->min ? time_echo_u : e->min;
	e->max = time_echo_u > e->max ? time_echo_u : e->max;
	return ;
}
