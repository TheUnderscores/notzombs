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

#ifndef NOTZOMBS_SERVERNET_H
#define NOTZOMBS_SERVERNET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void *get_address_str(struct sockaddr *a);

int get_address(struct addrinfo hints);

#endif /* #ifndef NOTZOMBS_SERVERNET_H */
