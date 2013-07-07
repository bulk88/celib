#ifndef ARPA_INET_H
#define ARPA_INET_H

#include <winsock.h>

struct in_addr	 inet_makeaddr(u_long , u_long);
unsigned long	 inet_netof(struct in_addr);

#endif
