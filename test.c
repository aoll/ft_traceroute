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
# include <sys/time.h>

# define PORT 33434
# define BUFSIZE 1024
# define MAX_TTL 30
# define NB_PACKET_BY_HOP 3
# define NB_PACKET_SEND 16
# define MAX_DELAY 5

typedef struct	s_msg
{
	int seq;
	int ttl;
}				t_msg;

int	init_fd_receve(void)
{
	int fd;                         /* our socket */

	if ((fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		perror("cannot create socket\n");
		exit (1);
	}
	// int optval = 1;
    // setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
	//
	// const int val = BUFSIZE;
	// if (setsockopt(fd, SOL_IP, SO_RCVBUF, &val, sizeof(val)) != 0)
	// {
	// 	printf("%s\n", "ERROR");
	// 	exit(EXIT_FAILURE);
	// }
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

int	receve(int fd)
{
	struct sockaddr_in myaddr;      /* our address */
	struct sockaddr_in remaddr;     /* remote address */
	socklen_t addrlen = sizeof(remaddr);	    /* length of addresses */
	int recvlen;	    /* # bytes received */
	unsigned char buf[BUFSIZE];     /* receive buffer */

	struct timeval	t_time_start;
	struct timeval	t_time_now;

	struct icmphdr *icmp;
	gettimeofday(&t_time_start, NULL);
	int size_min_pckt = sizeof(struct icmphdr) + sizeof(struct ip);
	int is_finish = 0;
	int received_packet = 0;
	t_msg *msg;


    while (42)
	{
		gettimeofday(&t_time_now, NULL);
		if (t_time_now.tv_sec >= t_time_start.tv_sec + MAX_DELAY && !is_finish)
		{
			printf("%s received_packet: %d\n", "* * *", received_packet);
			return (1);
		}
		// recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		bzero(buf, BUFSIZE);
		recvlen = recvfrom(fd, buf, BUFSIZE, MSG_DONTWAIT, (struct sockaddr *)&remaddr, &addrlen);
		if (recvlen > size_min_pckt )
		{
			printf("recvlen : %d ", recvlen);
			buf[recvlen] = 0;
			icmp = (struct icmphdr *)((void *)buf + sizeof(struct ip));
			struct ip *iph = (struct ip *)buf;
			// if (recvlen == 84)
			// {
			// 	write(1, buf + 8 + sizeof(struct ip) + sizeof(struct icmphdr) , recvlen - sizeof(struct ip) + sizeof(struct icmphdr) - 8);
			// }
			// else if (recvlen == 56)
			// {
			// 	write(1, buf + 8, 28);
			// }
			write(1, buf + 56, recvlen);
			msg = (void *)buf + 56;
			// msg = (void *)buf + (sizeof(struct ip) + sizeof(struct icmphdr));
			printf("ttl: %d, seq: %d, addr: %s, type: %d, code: %d\n",
				// msg->ttl, msg->seq,
				iph->ip_ttl,
				icmp->un.echo.sequence,
				inet_ntoa(remaddr.sin_addr), icmp->type, icmp->code);
			// if ((int)icmp->type == 3 &&  (int)icmp->code == 3)
			if (icmp->type == ICMP_DEST_UNREACH &&  icmp->code == ICMP_PORT_UNREACH)
			{
				// printf("%s\n", "END !!!!!!");
				is_finish++;
				if (is_finish >= NB_PACKET_BY_HOP)
				{
					exit(1);
				}
			}
			received_packet++;
			bzero(buf, BUFSIZE);
		}
		else if (recvlen > 0)
		{
			printf("%s %d\n", "! EXIT !", recvlen);
			exit(1);
		}
		// if (received_packet >= NB_PACKET_BY_HOP)
		// {
		// 	// printf("%s\n", "received_packet >= NB_PACKET_BY_HOP !!!!!!");
		// 	if (is_finish)
		// 	{
		// 		exit(1);
		// 	}
		// 	return (0);
		// }
	}
	if (is_finish)
	{
		exit(1);
	}
        /* never exits */
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
	int so_broadcast = 1;

	int z = setsockopt(fd,
    	SOL_SOCKET,
    	SO_BROADCAST,
    	&so_broadcast,
    	sizeof so_broadcast);

	if ( z )
    	perror("setsockopt(2)");
	return (0);
}


int	loop(int fd_receve, struct sockaddr_in *servaddr)
{
	int send;
	int ttl = 1;
	int sended_packet_by_hop;
	int sended_packet;
	char s[28];
	int fd;
	t_msg msg;
	// int
// NB_PACKET_SEND
	/* send a message to the server */
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
				bzero(s, 28);
				fd = init_fd_send();
				set_ttl(fd, ttl);
				msg.ttl = ttl;
				msg.seq = sended_packet_by_hop;
				// memcpy(s, "hello world!", strlen("hello world!"));
				memcpy(s, &msg, sizeof(t_msg));
				// t_msg *test;
				// test = (void *)s;
				// printf("test ttl: %d, seq: %d\n", test->ttl, test->seq);
				if ((send = sendto(fd, s, 28, 0, (struct sockaddr *)servaddr, sizeof(struct sockaddr))) < 0) {
					perror("sendto failed");
					return 0;
				}
				close(fd);
				sended_packet_by_hop++;
				sended_packet++;
				// sleep(1);
			}
			// ttl++;
		}
		receve(fd_receve);
		// printf("sended_packet_by_hop : %d  ", sended_packet_by_hop);
		// printf("%s%d  ", ttl<10?" ":"", ttl);
	}
	return (0);
}

int main(int argc, char const **av) {
	/* code */
	// int fd = init_fd_send();
	int fd_receve = init_fd_receve();



	// set_ttl(fd, 3);


	struct hostent *hp;     /* host information */
	struct sockaddr_in servaddr;    /* server address */
	char *my_message = "this is a trace route message";

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


	loop(fd_receve, &servaddr);
	// int send;
	// /* send a message to the server */
	// if ((send = sendto(fd, my_message, strlen(my_message), 0, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0) {
	// 	perror("sendto failed");
	// 	return 0;
	// }
	// printf("send : %d\n", send);
	//
	// receve(fd_receve);

	return 0;
}
