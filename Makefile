# Makefile - build script

# build environment
CC=gcc

# source files
COMMON_SOURCES := $(wildcard src/common/*.c)
CLIENT_SOURCES := $(COMMON_SOURCES) $(wildcard src/client/*.c)
SERVER_SOURCES := $(COMMON_SOURCES) $(wildcard src/server/*.c)

# object files
COMMON_OBJS := $(patsubst %.c,%.o,$(COMMON_SOURCES))
CLIENT_OBJS := $(patsubst %.c,%.o,$(CLIENT_SOURCES))
SERVER_OBJS := $(patsubst %.c,%.o,$(SERVER_SOURCES))

# link flags
COMMON_LINK :=
CLIENT_LINK := $(COMMON_LINK) -lSDL2 -lSDL2_net
SERVER_LINK := $(COMMON_LINK)

# Build flags
INCLUDES    := -I include

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
	$(CC) $(CFLAGS) $(CLIENT_OBJS) $(CLIENT_LINK) -o ct-client

server:  $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) $(SERVER_LINK) -o ct-server

debug: CFLAGS += -g
debug: all

clean:
	$(RM) $(COMMON_OBJS) $(CLIENT_OBJS) $(SERVER_OBJS) ct-client ct-server

