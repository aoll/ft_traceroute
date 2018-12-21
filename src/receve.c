/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 03:08:14 by alex              #+#    #+#             */
/*   Updated: 2018/12/21 03:14:00 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int	receve(int fd)
{
	struct sockaddr_in remaddr[NB_PACKET_BY_HOP];
	socklen_t addrlen = sizeof(remaddr);
	int recvlen;
	char buf[NB_PACKET_BY_HOP][BUFSIZE];
	struct timeval	t_time_receve[NB_PACKET_BY_HOP];
	struct timeval	t_time_start;
	struct timeval	t_time_now;
	struct icmphdr *icmp;
	int size_min_pckt = sizeof(struct icmphdr) + sizeof(struct ip);
	int is_finish = 0;
	int received_packet = 0;

	gettimeofday(&t_time_start, NULL);
	ft_bzero(buf, BUFSIZE * NB_PACKET_BY_HOP);
	ft_bzero(t_time_receve, sizeof(struct timeval) * NB_PACKET_BY_HOP);
	ft_bzero(remaddr, sizeof(struct sockaddr_in) * NB_PACKET_BY_HOP);
    while (42)
	{
		gettimeofday(&t_time_now, NULL);
		if (t_time_now.tv_sec >= t_time_start.tv_sec + MAX_DELAY && !is_finish)
		{
			printf(" %s\n", "* * *");
			return (EXIT_FAILURE);
		}
		recvlen = recvfrom(fd, buf[received_packet], BUFSIZE, MSG_DONTWAIT, (struct sockaddr *)&remaddr[received_packet], &addrlen);
		if (recvlen > size_min_pckt )
		{
			gettimeofday(&t_time_receve[received_packet], NULL);
			buf[received_packet][recvlen] = 0;
			icmp = (struct icmphdr *)((void *)buf[received_packet] + sizeof(struct ip));
			if (icmp->type == ICMP_DEST_UNREACH &&  icmp->code == ICMP_PORT_UNREACH)
			{
				is_finish++;
				received_packet++;
			}
			else if (icmp->type == ICMP_TIME_EXCEEDED &&  icmp->code == ICMP_EXC_TTL)
			{
				received_packet++;
			}
		}
		if (received_packet >= NB_PACKET_BY_HOP)
		{
			display_hop(buf, remaddr, t_time_receve,  &t_time_start);
			if (is_finish)
			{
				exit(EXIT_FAILURE);
			}
			return (EXIT_SUCCESS);
		}
	}
	return (EXIT_SUCCESS);
}
