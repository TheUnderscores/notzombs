#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#include <stdio.h>

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

	if (SDLNet_Init()) {
		show_SDLNet_error("Could not initialize SDL_net");
		return -1;
	}

	/* Get our network connection ready */
	IPaddress server_ip;
	SDLNet_ResolveHost(&server_ip, argv[1], 12397);

	if (server_ip.host == INADDR_NONE) {
		/* No real error, so just print to stderr */
		fprintf(stderr, "Could not resolve address\n");
		return -1;
	}

	UDPsocket client_socket = SDLNet_UDP_Open(0);

	if (client_socket == NULL) {
		show_SDLNet_error("Could not create UDP socket");
		return -1;
	}

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

	my_window = SDL_CreateWindow("Coms Test Client",
				     SDL_WINDOWPOS_CENTERED,
				     SDL_WINDOWPOS_CENTERED,
				     640, 480, 0);

	if (my_window == NULL) {
		show_SDL_error("Could not create window");
		return -1;
	}

	while (1) {
		SDL_Event client_event;

		SDL_WaitEventTimeout(&client_event, 500);

		if (client_event.type == SDL_QUIT) break;

		int recv_stat = SDLNet_UDP_Recv(client_socket, recv_packet);

		if (recv_stat == 1)
			printf("Got %d bytes - \"%s\"\n", recv_packet->len,
			       recv_packet->data);
		else if (recv_stat == -1)
			show_SDLNet_error("Could not get UDP packet");
	}
	
	SDL_DestroyWindow(my_window);

	SDLNet_FreePacket(recv_packet);
	
	SDLNet_Quit();
	SDL_Quit();
	return 0;
}
