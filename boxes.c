# include <SDL2/SDL.h>

typedef struct BoxMapEntry {
  char state[32];
  int frame;
  SDL_Rect rect;
} HitboxMapEntry;
