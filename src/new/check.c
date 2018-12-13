/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 00:37:59 by alex              #+#    #+#             */
/*   Updated: 2018/12/12 23:21:44 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

int	check_size(int bytes, t_env *e)
{
	if (bytes != PACKETSIZE_TOTAL)
	{
		if (e->option_v)
		{
			printf("%s\n", "Icomplete packet.");
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_pid(struct icmphdr *hdr2, t_env *e)
{
	if (hdr2->un.echo.id != e->pid)
	{
		if (e->option_v)
		{
			printf("%s\n", "Bad id.");
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_sum(struct icmphdr *hdr2, t_env *e)
{
	unsigned short cksum;

	cksum = hdr2->checksum;
	hdr2->checksum = 0;
	hdr2->checksum = checksum((unsigned short *)hdr2, PACKETSIZE);
	if (hdr2->checksum != cksum)
	{
		if (e->option_v)
		{
			printf("%s\n", "Checksum fail.");
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_data(char *data, t_env *e)
{
	if (ft_strncmp(data, MY_DATA, MY_DATA_LEN))
	{
		if (e->option_v)
		{
			printf("%s\n", "Corrupted data.");
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	check_type(struct icmphdr *hdr, int bytes, t_env *e)
{
	if (bytes < PACKETSIZE_DATA)
	{
		return (check_size(bytes, e));
	}
	if (hdr->type == ICMP_TIME_EXCEEDED)
	{
		printf("64 bytes from %s: icmp_seq=%d Time to live exceeded\n",
			e->ipv4, e->nb_packet_send);
		return (EXIT_FAILURE);
	}
	else if (hdr->type != ICMP_ECHOREPLY)
	{
		if (e->option_v)
		{
			printf("Bad icmp type: %d\n", hdr->type);
		}
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
