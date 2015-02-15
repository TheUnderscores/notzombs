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

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include <stdio.h>

/* Test */
#include "world_data_common.h"
/* EOF Test */

#define UNUSED(x) (void)(x)

void show_SDL_error(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, SDL_GetError());
}

void show_SDLNet_error(const char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, SDLNet_GetError());
}

int main(int argc, char **argv)
{
	UNUSED(argc);

	SDL_Window *my_window = NULL;
	SDL_Surface *my_surface = NULL;

	UNUSED(my_surface);

	if (SDL_Init(SDL_INIT_VIDEO)) {
		show_SDL_error("Could not initialize SDL");
		return -1;
	}

	puts("Video initialized");

	if (SDLNet_Init()) {
		show_SDLNet_error("Could not initialize SDL_net");
		return -1;
	}

	puts("Networking initialized");

	/* Get our network connection ready */
	IPaddress server_ip;
	
	if (SDLNet_ResolveHost(&server_ip, "127.0.0.1", 12397) == -1) {
		show_SDLNet_error("Could not resolve address");
		return -1;
	}

	UDPsocket client_socket;

	if (!(client_socket = SDLNet_UDP_Open(12397))) {
		show_SDLNet_error("Could not create UDP socket");
		return -1;
	}

	puts("Created UDP socket");

	if (SDLNet_UDP_Bind(client_socket, 0, &server_ip) == -1) {
		show_SDLNet_error("Could not bind socket to address");
		perror("Could not bind socket to address");
		return -1;
	}

	UDPpacket *recv_packet = SDLNet_AllocPacket(4096);

	if (recv_packet == NULL) {
		show_SDLNet_error("Could not allocate packet memory");
		return -1;
	}

	puts("Allocated UDP packet memory");

	my_window = SDL_CreateWindow("Not Zombs Client",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480, 0);

	if (my_window == NULL) {
		show_SDL_error("Could not create window");
		return -1;
	}
	
        
	
	
	puts("Created window");

	puts("Going into main loop");

	while (1) {
		SDL_Event client_event;

		SDL_WaitEventTimeout(&client_event, 500);

		if (client_event.type == SDL_QUIT) break;

		int recv_stat = SDLNet_UDP_Recv(client_socket, recv_packet);

		if (recv_stat == 1)
			printf("Got %d bytes - \"%s\"\n", recv_packet->len,
			       recv_packet->data);
		else if (recv_stat == 0)
			show_SDLNet_error("No UDP packet received");
		else if (recv_stat == -1)
			show_SDLNet_error("Could not get UDP packet");
	}
	
	SDL_DestroyWindow(my_window);

	SDLNet_FreePacket(recv_packet);
	
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}
