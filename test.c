#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>	/* for fprintf */
#include <string.h>	/* for memcpy */


# include <sys/socket.h>
# include <netinet/ip.h>
# include <netinet/tcp.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <resolv.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip_icmp.h>
# include <netinet/tcp.h>
# include <netinet/udp.h>
# include <sys/time.h>
# include "libft.h"

# define PORT 33434
# define BUFSIZE 1024
# define MAX_TTL 30
# define NB_PACKET_BY_HOP 3
# define NB_PACKET_SEND 3
# define MAX_DELAY 5
# define SIZE_PACKET_SEND 60


int	init_fd_receve(void)
{
	int fd;                         /* our socket */

	if ((fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		perror("cannot create socket\n");
		exit (1);
	}
	return (fd);
}

int init_fd_send(void)
{
	int fd;

	if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("cannot create socket");
		exit(1);
	}
	return fd;
}

int	display(char *buf, struct sockaddr_in *remaddr,
	struct timeval *t_time_receve,
	struct timeval *t_time_start, int is_displayed_addr)
{
	struct icmphdr *icmp;
	icmp = (struct icmphdr *)((void *)buf + sizeof(struct ip));
	struct ip *iph = (struct ip *)buf;
	if (!is_displayed_addr)
	{
		printf(" %s",inet_ntoa(remaddr->sin_addr));
	}

	double			time_echo_u;
	time_echo_u = ((1000000 * t_time_receve->tv_sec + t_time_receve->tv_usec)
		- (1000000 * t_time_start->tv_sec + t_time_start->tv_usec)) ;
	printf("  %0.003f ms", time_echo_u / 1000);
	return (EXIT_SUCCESS);
}

int	is_diff_addr(struct sockaddr_in remaddr[], int index)
{
	int is_displayed;
	int tmp;

	is_displayed = 0;
	tmp = 0;
	while (tmp < index)
	{
		if (remaddr[index].sin_addr.s_addr == remaddr[tmp].sin_addr.s_addr)
		{
			is_displayed++;
		}
		tmp++;
	}
	return (is_displayed);
}

int	display_hop(char buf[NB_PACKET_BY_HOP][BUFSIZE],
	struct sockaddr_in *remaddr,
	struct timeval *t_time_receve,
	struct timeval *t_time_start)
{
	int	i;

	i = 0;
	while (i < NB_PACKET_BY_HOP)
	{
		display(buf[i], &remaddr[i], &t_time_receve[i] ,t_time_start, is_diff_addr(remaddr, i));
		i++;
	}
	printf("\n");
	return (0);
}

int	receve(int fd)
{
	struct sockaddr_in remaddr[NB_PACKET_BY_HOP];     /* remote address */
	socklen_t addrlen = sizeof(remaddr);	    /* length of addresses */
	int recvlen;	    /* # bytes received */
	char buf[NB_PACKET_BY_HOP][BUFSIZE];     /* receive buffer */

	struct timeval	t_time_receve[NB_PACKET_BY_HOP];

	struct timeval	t_time_start;
	struct timeval	t_time_now;

	struct icmphdr *icmp;
	gettimeofday(&t_time_start, NULL);
	int size_min_pckt = sizeof(struct icmphdr) + sizeof(struct ip);
	int is_finish = 0;
	int received_packet = 0;



	bzero(buf, BUFSIZE);
    while (42)
	{
		gettimeofday(&t_time_now, NULL);
		if (t_time_now.tv_sec >= t_time_start.tv_sec + MAX_DELAY && !is_finish)
		{
			printf(" %s\n", "* * *");
			return (1);
		}
		recvlen = recvfrom(fd, buf[received_packet], BUFSIZE, MSG_DONTWAIT, (struct sockaddr *)&remaddr[received_packet], &addrlen);
		if (recvlen > size_min_pckt )
		{
			gettimeofday(&t_time_receve[received_packet], NULL);
			buf[received_packet][recvlen] = 0;
			icmp = (struct icmphdr *)((void *)buf[received_packet] + sizeof(struct ip));
			struct ip *iph = (struct ip *)buf[received_packet];
			if (icmp->type == ICMP_DEST_UNREACH &&  icmp->code == ICMP_PORT_UNREACH)
			{
				is_finish++;
			}
			received_packet++;
		}
		if (received_packet >= NB_PACKET_BY_HOP)
		{
			display_hop(buf, remaddr, t_time_receve,  &t_time_start);
			if (is_finish)
			{
				exit(1);
			}
			return (0);
		}
	}
	return (0);
}


int			set_ttl(int fd, int ttl)
{
	const int	val = ttl;

	if (setsockopt(fd, IPPROTO_IP, IP_TTL, &val, sizeof(val)) != 0)
	{
		printf("%s\n", "ERROR");
		exit(1);
	}
	return (0);
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
				bzero(buf, SIZE_PACKET_SEND);
				fd = init_fd_send();
				set_ttl(fd, ttl);

				if ((send = sendto(fd, buf, SIZE_PACKET_SEND, 0, (struct sockaddr *)servaddr, sizeof(struct sockaddr))) < 0) {
					perror("sendto failed");
					return 0;
				}
				close(fd);
				sended_packet_by_hop++;
				sended_packet++;
			}
		}
		printf(" %s%d ", ((ttl < 10) ? " " : ""), ttl );
		receve(fd_receve);
	}
	return (0);
}

int main(int argc, char const **av) {
	int fd_receve = init_fd_receve();

	struct hostent *hp;     /* host information */
	struct sockaddr_in servaddr;    /* server address */

	/* fill in the server's address and data */
	memset((char*)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);

	/* look up the address of the server given its name */
	hp = gethostbyname(av[1]);
	if (!hp) {
		fprintf(stderr, "could not obtain address of %s\n", av[1]);
		return 0;
	}

	/* put the host's address into the server address structure */
	memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
	printf("traceroute to %s (%s), %d hops max, %d byte packets\n",
		av[1], "42.42.42.42", MAX_TTL, SIZE_PACKET_SEND);
	loop(fd_receve, &servaddr);

	return 0;
}
