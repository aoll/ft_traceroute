/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/07 16:58:49 by alex              #+#    #+#             */
/*   Updated: 2018/12/13 05:49:15 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"


int			set_ttl(t_env *e, int ttl)
{
	const int	val = ttl;

	if (setsockopt(e->socket, SOL_IP, IP_TTL, &val, sizeof(val)) != 0)
	{
		printf("%s\n", "ERROR");
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int			loop(t_env *e)
{
	int ttl;

	init_packet(e);
	ttl = 1;
	init_packet(e);
	while (ttl <= MAX_TTL)
	{

		printf("ttl: %d\n", ttl);
		set_ttl(e, ttl);
		send_packet(e);
		send_packet(e);
		send_packet(e);
		if (read_packet(e) == 2)
			return (EXIT_SUCCESS);
		if (read_packet(e) == 2)
			return (EXIT_SUCCESS);
		if (read_packet(e) == 2)
			return (EXIT_SUCCESS);
		ttl++;
	}
	return (EXIT_SUCCESS);
}

void		check(int ac, char **av, t_env *e)
{
	int i;
	int j;
	int nb_dest;

	i = 0;
	nb_dest = 0;
	while (++i < ac)
		if (av[i][0] == '-')
		{
			if (!av[i][1])
				usage();
			j = 0;
			while (av[i][++j])
				if (av[i][j] == 'h')
					usage();
				else if (av[i][j] == 'v')
					e->option_v = 1;
		}
		else
		{
			e->adr = av[i];
			nb_dest++;
		}
	if (ac < 2 || nb_dest != 1)
		usage();
}

int			main(int ac, char **av)
{
	struct sockaddr	*ad_dst;
	t_env			e;

	if (getuid())
	{
		printf("%s\n", "Must be root.");
		return (EXIT_FAILURE);
	}
	g_is_stop = 0;
	e.adr = av[1];
	// g_ttl = ft_atoi(av[1]);
	init_env(ac, av, &e);
	// signal(SIGINT, int_handler);
	// printf("PING %s (%s) 56(84) bytes of data.\n", e.adr, e.ipv4);
	return (loop(&e));
}
