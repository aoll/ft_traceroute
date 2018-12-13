/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 00:50:20 by alex              #+#    #+#             */
/*   Updated: 2018/12/11 09:15:27 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int	send_packet(t_env *e)
{
	ssize_t sent_bytes;

	gettimeofday(&e->pck.t_time, NULL);
	e->pck.hdr.un.echo.sequence = e->seq;
	e->pck.hdr.checksum = 0;
	e->pck.hdr.checksum = checksum((unsigned short *)&e->pck, PACKETSIZE);
	sent_bytes = sendto(e->socket, &e->pck.hdr, PACKETSIZE, 0,
		(struct sockaddr *)e->ad_dst, sizeof(struct sockaddr));
	if (sent_bytes != PACKETSIZE)
	{
		printf("%s\n", "ft_ping: sendto: Network is unreachable");
	}
	e->nb_packet_send++;
	e->seq++;
	return (EXIT_SUCCESS);
}
