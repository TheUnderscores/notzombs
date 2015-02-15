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

#include "packet.h"
#include "server_network.h"
#include "server_error.h"

#define MAX_DATA 4096

int main(int argc, char **argv)
{
	int server_sock;
	struct addrinfo hints;
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

	server_sock = get_address(hints);

	if (server_sock == -1) {
		perror("Could not get address");
		return -1;
	}

	addr_len = sizeof(client_addr);

	puts("Entering main loop...\n");
	
	while (1) {
		int packet_size = recvfrom(server_sock, data, MAX_DATA - 1,
			0, (struct sockaddr *)&client_addr, &addr_len);

		if (packet_size == -1) {
			perror("Error receiving packet");
			return -2;
		}

		/* Limit string */
		data[packet_size] = '\0';

		inet_ntop(client_addr.ss_family,
			get_address_str((struct sockaddr *)&client_addr),
			ipstr, sizeof(ipstr));

		printf("Got packet from %s\n", ipstr);
		printf("Packet is %d bytes and contains \"%s\"\n\n",
			packet_size, data);

		JSON_Value *json_packet = json_parse_string(data);

		if (json_packet == NULL) {
			fprintf(stderr, "Could not parse JSON packet\n");
			return -3;
		}

		if (json_type(json_packet) != JSONObject)
			fprintf(stderr, "json_pack is not an object.\n");

		JSON_Object *packet_obj = json_object(json_packet);

		printf("Packet type is \"%s\"\n", packet_get_type(packet_obj));

		struct version_t client_ver = packet_get_version(packet_obj);
		
		printf("Reported version : %d.%d.%d\n", client_ver.major,
		       client_ver.minor, client_ver.patch);

		printf("\n");
	}
}
