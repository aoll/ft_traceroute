/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/21 03:05:48 by alex              #+#    #+#             */
/*   Updated: 2018/12/21 03:13:12 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

static void	usage(void)
{
	printf("%s\n", "Usage: ft_traceroute [-h] [-h help] destination");
	exit(EXIT_FAILURE);
}

static void	check(int ac, char **av)
{
	int i;
	int nb_dest;

	i = 0;
	nb_dest = 0;
	while (++i < ac)
		if (av[i][0] == '-')
		{
			usage();
		}
		else
		{
			nb_dest++;
		}
	if (ac < 2 || nb_dest != 1)
		usage();
}

int			main(int ac, char **av)
{
	int					fd_receve;
	struct sockaddr_in	*servaddr;
	char				buf[INET_ADDRSTRLEN];

	if (getuid())
	{
		printf("%s\n", "Must be root.");
		return (EXIT_FAILURE);
	}
	check(ac, av);
	fd_receve = init_fd_receve();
	servaddr = get_addr(av[1]);
	servaddr->sin_port = htons(PORT);
	if (!inet_ntop(AF_INET, &servaddr->sin_addr,
		buf, INET_ADDRSTRLEN))
	{
		printf("Error to get adress.\n");
		exit(EXIT_FAILURE);
	}
	printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
		av[1], buf, MAX_TTL, SIZE_PACKET_SEND);
	loop(fd_receve, servaddr);
	return (EXIT_SUCCESS);
}
