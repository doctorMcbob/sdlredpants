#include "uthash.h"
#include "worlds.c"
#include "actors.c"

typedef struct Frame {
  int scroll_x;
  int scroll_y;
  int x;
  int y;
  int w;
  int h;
  World* world;
  Actor* focus;
  int bound_left;
  int bound_top;
  int bound_right;
  int bound_bottom;
  int active;
} Frame;
