/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 03:05:33 by alex              #+#    #+#             */
/*   Updated: 2018/12/21 03:05:54 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

struct sockaddr_in		*get_addr(const char *adr)
{
	struct addrinfo	hints;
	struct addrinfo	*res;

	ft_memset(&hints, 0, sizeof(hints));
	hints.ai_flags = 0;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	if (getaddrinfo(adr, 0, &hints, &res) < 0)
	{
		printf("ft_ping: unknown host %s\n", adr);
		exit(EXIT_FAILURE);
	}
	return ((struct sockaddr_in *)res->ai_addr);
}

int	init_fd_receve(void)
{
	int fd;                         /* our socket */

	if ((fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		perror("cannot create socket\n");
		exit (EXIT_FAILURE);
	}
	return (fd);
}

int init_fd_send(void)
{
	int fd;

	if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("cannot create socket");
		exit(EXIT_FAILURE);
	}
	return fd;
}
