/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 07:42:54 by alex              #+#    #+#             */
/*   Updated: 2018/12/11 09:28:31 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

double	ft_sqrtl(double x)
{
	double n;

	n = x / 2;
	while (ABS(n * n - x) > TOLERANCE)
	{
		n = (n + x / n) / 2;
	}
	return (n);
}

void	usage(void)
{
	printf("%s\n", "Usage: ft_ping [-hv] [-h help] [-v verbose] destination");
	exit(EXIT_FAILURE);
	return ;
}

void	display_nb_packets(t_env *e)
{
	struct timeval	t_time;

	gettimeofday(&t_time, NULL);
	printf("%d packets transmitted, ", e->nb_packet_send);
	printf("%d received, ", e->nb_packet_rcv);
	if (e->nb_packet_error)
		printf("+%d errors, ", e->nb_packet_error);
	printf("%d%s packet loss, ",
	(e->nb_packet_rcv != e->nb_packet_send ?
		((e->nb_packet_send - e->nb_packet_rcv) / e->nb_packet_send * 100) : 0),
		"%");
	printf("time %.0fms\n", (float)((1000000 * t_time.tv_sec + t_time.tv_usec)
		- (1000000 * (e->start_time.tv_sec) + e->start_time.tv_usec)) / 1000);
	return ;
}

void	display_statistics(t_env *e)
{
	printf("\n--- %s ft_ping statistics ---\n", e->adr);
	display_nb_packets(e);
	if (!e->nb_packet_rcv)
	{
		printf("\n");
		return ;
	}
	printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
		(e->min), (e->total / e->nb_packet_rcv), (e->max),
		ft_sqrtl((e->tsum / (e->nb_packet_rcv + e->nb_packet_send)) -
		(e->total / (e->nb_packet_rcv + e->nb_packet_send)) *
		(e->total / (e->nb_packet_rcv + e->nb_packet_send))));
	return ;
}
