/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 03:07:21 by alex              #+#    #+#             */
/*   Updated: 2018/12/21 03:07:45 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

int	set_ttl(int fd, int ttl)
{
	const int	val = ttl;

	if (setsockopt(fd, IPPROTO_IP, IP_TTL, &val, sizeof(val)) != 0)
	{
		printf("%s\n", "ERROR");
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}


int	loop(int fd_receve, struct sockaddr_in *servaddr)
{
	int send;
	int ttl = 1;
	int sended_packet_by_hop;
	int sended_packet;
	char buf[SIZE_PACKET_SEND];
	int fd;

	sended_packet_by_hop = 0;
	while (ttl <= MAX_TTL)
	{

		sended_packet = 0;
		while (sended_packet < NB_PACKET_SEND)
		{
			if (sended_packet_by_hop == NB_PACKET_BY_HOP)
			{
				sended_packet_by_hop = 0;
				ttl++;
			}
			while (sended_packet_by_hop < NB_PACKET_BY_HOP && sended_packet < NB_PACKET_SEND)
			{
				ft_bzero(buf, SIZE_PACKET_SEND);
				fd = init_fd_send();
				servaddr->sin_port = htons(PORT + (ttl - 1) * NB_PACKET_BY_HOP + sended_packet_by_hop + 1);
				set_ttl(fd, ttl);
				if ((send = sendto(fd, buf, SIZE_PACKET_SEND, 0, (struct sockaddr *)servaddr, sizeof(struct sockaddr))) < 0) {
					printf("sendto failed");
					exit(EXIT_FAILURE);
				}
				close(fd);
				sended_packet_by_hop++;
				sended_packet++;
			}
		}
		printf(" %s%d ", ((ttl < 10) ? " " : ""), ttl );
		receve(fd_receve);
	}
	return (EXIT_SUCCESS);
}
