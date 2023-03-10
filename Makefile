# A simple Makefile for SDL projects

# set the compiler
CC := clang

# set compiler flags #! REMOVE -g !# 
SDL_FLAGS := `sdl2-config --libs --cflags`
CFLAGS := $(SDL_FLAGS) -ggdb3 -O0 --std=c99 -Wall

# add headers here
HDRS :=

# add source files here
SRCS := sprites.c spritesheets.c main.c inputs.c actors.c actordata.c worlds.c worlddata.c frames.c boxes.c boxdata.c scripts.c scriptdata.c

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := game

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS) -lSDL2_image -lm

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#	$(CC) -c $(CFLAGS) -o $@ $(@:.o=.c)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean

