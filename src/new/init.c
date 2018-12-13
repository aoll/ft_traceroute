/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 07:40:10 by alex              #+#    #+#             */
/*   Updated: 2018/12/13 03:24:55 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void				int_handler(int sig)
{
	g_is_stop = 1;
}

int					init_socket(void)
{
	int			s;
	// const int	val = g_ttl;
	// const int	val = MY_TTL;

	//val = MY_TTL;
	s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	// if (setsockopt(s, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
	// {
	// 	printf("%s\n", "ERROR");
	// 	exit(EXIT_FAILURE);
	// }
	return (s);
}

struct sockaddr		*get_addr(const char *adr, t_env *e)
{
	struct addrinfo	hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = 0;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(adr, 0, &hints, &e->res) < 0)
	{
		printf("ft_ping: unknown host %s\n", adr);
		exit(EXIT_FAILURE);
	}
	if (!inet_ntop(AF_INET, &((struct sockaddr_in *)e->res->ai_addr)->sin_addr,
		e->ipv4, INET_ADDRSTRLEN))
	{
		printf("Error to get adress.\n");
		exit(EXIT_FAILURE);
	}
	return (e->res->ai_addr);
}

void				init_env(int ac, char **av, t_env *e)
{
	gettimeofday(&e->start_time, NULL);
	e->pid = getpid();
	e->seq = 1;
	e->option_t = 0;
	e->option_v = 0;
	e->nb_packet_send = 0;
	e->nb_packet_rcv = 0;
	e->nb_packet_error = 0;
	e->min = 0;
	e->max = 0;
	e->total = 0;
	ft_bzero(e->ipv4, INET_ADDRSTRLEN);
	// check(ac, av, e);
	e->socket = init_socket();
	e->ad_dst = get_addr(e->adr, e);
	return ;
}
