/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   receve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 00:51:09 by alex              #+#    #+#             */
/*   Updated: 2018/12/13 05:39:17 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int	process_packet(unsigned char buf[], int bytes, t_env *e)
{
	if (
		check_type(
			(struct icmphdr *)((void *)buf + sizeof(struct ip)), bytes, e)
		|| check_size(bytes, e)
		|| check_pid((struct icmphdr *)((void *)buf + sizeof(struct ip)), e)
		|| check_sum((struct icmphdr *)((void *)buf + sizeof(struct ip)), e)
		|| check_data((char *)buf + (PACKETSIZE_TOTAL - MY_DATA_LEN), e))
	{
		e->nb_packet_error++;
		return (EXIT_FAILURE);
	}
	display((struct icmphdr *)((void *)buf + sizeof(struct ip)),
	(struct timeval *)(
		(void *)buf + sizeof(struct ip) + sizeof(struct icmphdr)),
		(struct ip *)buf, e);
	e->nb_packet_rcv++;
	return (EXIT_SUCCESS);
}

// int						read_packet(t_env *e)
// {
// 	struct msghdr	msg;
// 	struct iovec	iov;
// 	unsigned char	iov_base[PACKETSIZE_TOTAL];
// 	ssize_t			ret;
//
// 	msg.msg_name = e->res->ai_addr;
// 	msg.msg_namelen = e->res->ai_addrlen;
// 	msg.msg_iov = &iov;
// 	msg.msg_iovlen = 1;
// 	iov.iov_base = &iov_base;
// 	iov.iov_len = PACKETSIZE_TOTAL;
// 	ret = recvmsg(e->socket, &msg, MSG_DONTWAIT);
// 	if (ret > 0)
// 	{
// 		return (process_packet(iov_base, ret, e));
// 	}
// 	return (EXIT_SUCCESS);
//
// 	// if (ret	< 0)
// 	// 	return (1);
// 	//
// 	// return (validate_msg(iov_base, ret));
// }
//

/*
**
*/

int		test(struct sockaddr_in	*addr)
{
	printf("%s\n", inet_ntoa(addr->sin_addr));

	return (ft_strcmp(inet_ntoa(addr->sin_addr), "216.58.215.46") ? 1 : 2);
	// return (1);
}

int			read_packet2(t_env *e)
{
	unsigned char		buf[1024];
	struct sockaddr_in	addr;
	int					bytes;
	unsigned int		len;

	len = sizeof(struct sockaddr_in);
	ft_bzero(buf, sizeof(buf));
	bytes = recvfrom(e->socket, buf, sizeof(buf), MSG_DONTWAIT,
		(struct sockaddr*)&addr, &len);
	if (bytes > 0)
	{
		return (test(&addr));
	// 	return (process_packet(buf, bytes, e));
	}
	return (EXIT_SUCCESS);
}

int read_packet(t_env *e)
{
	struct timeval	t_time_start;
	struct timeval	t_time_now;
	int 			ret;
	int				count;

	count = 0;
	gettimeofday(&t_time_start, NULL);
	while (42)
	{
		if ((ret = read_packet2(e)))
			return (ret);
		gettimeofday(&t_time_now, NULL);
		if (t_time_now.tv_sec >= t_time_start.tv_sec + 2)
		{
			printf("%s\n", "* * *");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}
