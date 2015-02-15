/*
 * notzombs - top-down, procedurally generated zombie survival game
 * Copyright (C) 2015 Robert Cochran and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "server_network.h"
#include "server_error.h"

#define SERVER_PORT "12397"

/*
 * Technically, we do not need this function, because
 * SDL_net can only use IPv4 addresses. /However/, this function
 * is good because a) it's a simple call for the raw address data,
 * and b) we're ready for IPv6 support when it comes into SDL_net.
 */

void *get_address_str(struct sockaddr *a)
{
	if (a->sa_family == AF_INET)
	       	return &(((struct sockaddr_in *)a)->sin_addr);
	else
		return &(((struct sockaddr_in6 *)a)->sin6_addr);
}

int get_address(struct addrinfo hints)
{
	struct addrinfo *info, *p;
	int s;
	
	int ret = getaddrinfo(NULL, SERVER_PORT, &hints, &info);

	if (ret != 0) {
		show_network_error("Could not get address info", ret);
		return -1;
	}

	/* Find a valid address */
	for (p = info; p != NULL; p = p->ai_next) {
		s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

		if (s == -1) {
			perror("Could not create socket");
			continue;
		}

		int optval = 1;

		setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval,
			sizeof(optval));

		if (optval != 1) {
			perror("Could not set socket to reuse address");
			continue;
		}

		if (bind(s, p->ai_addr, p->ai_addrlen) == -1) {
			close(s);
			perror("Could not bind to socket");
			continue;
		}

		/* We got this far, and thus found a match. Take it. */
		break;	
	}

	freeaddrinfo(info);

	if (p == NULL)
		return -1;
	else
		return s;
}
