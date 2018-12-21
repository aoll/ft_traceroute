#ifndef FT_TRAROUTE_H
# define FT_TRAROUTE_H

# include <sys/types.h>
# include <sys/socket.h>

# include <stdio.h>


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

struct sockaddr_in		*get_addr(const char *adr);
int						init_fd_receve(void);
int 					init_fd_send(void);
int						display_hop(char buf[NB_PACKET_BY_HOP][BUFSIZE],
									struct sockaddr_in *remaddr,
									struct timeval *t_time_receve,
									struct timeval *t_time_start);
int						receve(int fd);
int						loop(int fd_receve, struct sockaddr_in *servaddr);

#endif
