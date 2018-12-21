/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 03:03:34 by alex              #+#    #+#             */
/*   Updated: 2018/12/21 03:13:23 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static int	display(char *buf, struct sockaddr_in *remaddr,
	struct timeval *t_time_receve,
	struct timeval *t_time_start, int is_displayed_addr)
{
	struct icmphdr *icmp;
	icmp = (struct icmphdr *)((void *)buf + sizeof(struct ip));
	if (!is_displayed_addr)
	{
		printf(" %s",inet_ntoa(remaddr->sin_addr));
	}

	double			time_echo_u;
	time_echo_u = ((1000000 * t_time_receve->tv_sec + t_time_receve->tv_usec)
		- (1000000 * t_time_start->tv_sec + t_time_start->tv_usec)) ;
	printf("  %0.003f ms", time_echo_u / 1000);
	return (EXIT_SUCCESS);
}

static int	is_diff_addr(struct sockaddr_in remaddr[], int index)
{
	int is_displayed;
	int tmp;

	is_displayed = 0;
	tmp = 0;
	while (tmp < index)
	{
		if (remaddr[index].sin_addr.s_addr == remaddr[tmp].sin_addr.s_addr)
		{
			is_displayed++;
		}
		tmp++;
	}
	return (is_displayed);
}

int			display_hop(char buf[NB_PACKET_BY_HOP][BUFSIZE],
	struct sockaddr_in *remaddr,
	struct timeval *t_time_receve,
	struct timeval *t_time_start)
{
	int	i;

	i = 0;
	while (i < NB_PACKET_BY_HOP)
	{
		display(buf[i], &remaddr[i], &t_time_receve[i] ,t_time_start, is_diff_addr(remaddr, i));
		i++;
	}
	printf("\n");
	return (EXIT_SUCCESS);
}
