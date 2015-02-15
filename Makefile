# notzombs - top-down, procedurally generated zombie survival game
# Copyright (C) 2015 Robert Cochran and others
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Makefile - build script

# build environment
CC=gcc

# source files
COMMON_SOURCES := $(wildcard src/common/*.c) src/common/parson/parson.c
CLIENT_SOURCES := $(COMMON_SOURCES) $(wildcard src/client/*.c)
SERVER_SOURCES := $(COMMON_SOURCES) $(wildcard src/server/*.c)

# object files
COMMON_OBJS := $(patsubst %.c,%.o,$(COMMON_SOURCES))
CLIENT_OBJS := $(patsubst %.c,%.o,$(CLIENT_SOURCES))
SERVER_OBJS := $(patsubst %.c,%.o,$(SERVER_SOURCES))

# link flags
COMMON_LINK :=
CLIENT_LINK := $(COMMON_LINK) -lSDL2 -lSDL2_net -lSDL2_image
SERVER_LINK := $(COMMON_LINK)

# Build flags
INCLUDES    := -I./include/ -I./src/common/parson/

#BASEFLAGS   := -O2
WARNFLAGS   := -Wall -Wextra -Wshadow -Wcast-align -Wwrite-strings -Winline
WARNFLAGS   += -Wno-attributes -Wno-deprecated-declarations
WARNFLAGS   += -Wno-div-by-zero -Wno-endif-labels -Wfloat-equal
WARNFLAGS   += -Wformat=2 -Wno-format-extra-args -Winit-self
WARNFLAGS   += -Winvalid-pch -Wmissing-format-attribute
WARNFLAGS   += -Wmissing-include-dirs -Wno-multichar -Wshadow
WARNFLAGS   += -Wno-sign-compare -Wswitch -Wsystem-headers
WARNFLAGS   += -Wno-pragmas -Wno-unused-but-set-parameter
WARNFLAGS   += -Wno-unused-but-set-variable -Wno-unused-result
WARNFLAGS   += -Wwrite-strings -Wdisabled-optimization -Wpointer-arith
#WARNFLAGS   += -Werror
CFLAGS      := $(INCLUDES) $(BASEFLAGS) $(WARNFLAGS)
CFLAGS      += -std=gnu99

# build rules
all: client server

client:  $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) $(CLIENT_LINK) -o nz-client

server:  $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) $(SERVER_LINK) -o nz-server

debug: CFLAGS += -g
debug: all

clean:
	$(RM) $(COMMON_OBJS) $(CLIENT_OBJS) $(SERVER_OBJS) nz-client nz-server

