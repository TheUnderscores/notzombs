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

#include "packet.h"

const char *packet_get_type(JSON_Object *p)
{
	return json_object_get_string(p, "type");
}

struct version_t packet_get_version(JSON_Object *p)
{
	return (struct version_t) {
		.major = json_object_dotget_number(p, "version.major"),
		.minor = json_object_dotget_number(p, "version.minor"),
		.patch = json_object_dotget_number(p, "version.patch"),
	};
}
