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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "parson.h"

#define SERVER_PORT "12397"

#define MAX_DATA 4096

void show_network_error(const char *s, int r)
{
	fprintf(stderr, "%s: %s\n", s, gai_strerror(r));
}

/*
 * Technically, we do not need this function, because
 * SDL_net can only use IPv4 addresses. /However/, this function
 * is good because a) it's a simple call for the raw address data,
 * and b) we're ready for IPv6 support when it comes into SDL_net.
 */

void *get_address(struct sockaddr *a)
{
	if (a->sa_family == AF_INET)
	       	return &(((struct sockaddr_in *)a)->sin_addr);
	else
		return &(((struct sockaddr_in6 *)a)->sin6_addr);
}

int main(int argc, char **argv)
{
	int server_sock;
	struct addrinfo hints, *server_info, *p;
	struct sockaddr_storage client_addr;
	char data[MAX_DATA];
	socklen_t addr_len;
	char ipstr[INET6_ADDRSTRLEN];

	puts("Initializing networking...");

	/* Fill in hints */
	memset(&hints, 0, sizeof(hints));

	/* IP address *must* be IPv4, because SDL_net only does IPv4 */
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; /* Get our IP */

	int ret = getaddrinfo(NULL, SERVER_PORT, &hints, &server_info);

	if (ret != 0) {
		show_network_error("Could not get address info", ret);
		return -1;
	}

	/* Find a valid address */
	for (p = server_info; p != NULL; p = p->ai_next) {
		server_sock = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol);

		if (server_sock == -1) {
			perror("Could not create socket");
			continue;
		}

		int optval = 1;

		setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval,
			sizeof(optval));

		if (optval != 1) {
			perror("Could not set socket to reuse address");
			continue;
		}

		if (bind(server_sock, p->ai_addr, p->ai_addrlen) == -1) {
			close(server_sock);
			perror("Could not bind to socket");
			continue;
		}

		/* We got this far, and thus found a match. Take it. */
		break;	
	}

	if (p == NULL) {
		fprintf(stderr, "Exhausted all addresses. Bailing out.\n");
		return -2;
	}

	freeaddrinfo(server_info);

	addr_len = sizeof(client_addr);

	puts("Entering main loop...\n");
	
	while (1) {
		int packet_size = recvfrom(server_sock, data, MAX_DATA - 1,
			0, (struct sockaddr *)&client_addr, &addr_len);

		if (packet_size == -1) {
			perror("Error receiving packet");
			return -3;
		}

		/* Limit string */
		data[packet_size] = '\0';

		inet_ntop(client_addr.ss_family,
			get_address((struct sockaddr *)&client_addr),
			ipstr, sizeof(ipstr));

		printf("Got packet from %s\n", ipstr);
		printf("Packet is %d bytes and contains \"%s\"\n\n",
			packet_size, data);

		JSON_Value *json_packet = json_parse_string(data);

		if (json_packet == NULL) {
			fprintf(stderr, "Could not parse JSON packet\n");
			return;
		}

		if (json_type(json_packet) != JSONObject)
			fprintf(stderr, "json_pack is not an object.\n");

		JSON_Object *packet_obj = json_object(json_packet);

		printf("Packet type is \"%s\"\n",
		       json_object_get_string(packet_obj, "type"));

		int version_major = (int) json_object_dotget_number(packet_obj,
			"version.major");
		int version_minor = (int) json_object_dotget_number(packet_obj,
			"version.minor");
		int version_patch = (int) json_object_dotget_number(packet_obj,
			"version.patch");

		printf("Reported version : %d.%d.%d\n",
		       version_major, version_minor, version_patch);

		printf("\n");
	}
}
