# include <SDL2/SDL.h>
# include "uthash.h"
# include <stdio.h>
# include <string.h>

typedef struct Actor {
  char name[32];
  SDL_Rect* ECB;

  float x_vel;
  float y_vel;
  // hitboxes, hurtboxes
  // attributes dictionary
  // spritesheet
  char img[32];
  char offsetkey[32];
  char _input_name[32];
  char state[16];
  int frame;
  int direction;
  int rotation;
  // flags
  int platform;
  int tangible;
  int physics;
  int updated;
  UT_hash_handle hh;
} Actor;

Actor* actors = NULL;

Actor* get_actor(const char* name) {
  struct Actor *a;
  HASH_FIND_STR(actors, name, a);
  if (a) {
    return a;
  } else {
    return NULL;
  }
}


