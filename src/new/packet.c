/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/09 23:21:08 by alex              #+#    #+#             */
/*   Updated: 2018/12/11 00:52:00 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

unsigned short	checksum(unsigned short *buffer, int size)
{
	unsigned int cksum;

	cksum = 0;
	while (size > 1)
	{
		cksum += *buffer++;
		size -= sizeof(unsigned short);
	}
	if (size)
		cksum += *(unsigned char *)buffer;
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (unsigned short)(~cksum);
}

int				init_packet(t_env *e)
{
	ft_memset(&e->pck, 0, PACKETSIZE);
	e->pck.hdr.type = ICMP_ECHO;
	e->pck.hdr.code = 0;
	e->pck.hdr.un.echo.id = e->pid;
	ft_memcpy(e->pck.data, MY_DATA, ft_strlen(MY_DATA));
	return (EXIT_SUCCESS);
}
