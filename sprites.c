/**
   The sprites module

its really a dictionary of name -> sprite (SDL_Texture)

Using the implementation of a dictionary like hash `uthash.h`
https://github.com/troydhanson/uthash/blob/master/src/uthash.h

*/

# include "uthash.h"
# include "utlist.h"
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>

typedef struct Sprite {
  char name[32];
  SDL_Texture* image;
  UT_hash_handle hh;
} Sprite;

typedef struct Offset {
  char name[32];
  int x;
  int y;
  UT_hash_handle hh;
} Offset;

typedef struct SpriteMapEntry {
  char state[32];
  int frame;
  char spriteKey[32];
  struct SpriteMapEntry* next;
  struct SpriteMapEntry* prev;
} SpriteMapEntry;

typedef struct SpriteMap {
  char* name[32];
  SpriteMapEntry* entries;
  UT_hash_handle hh;
} SpriteMap;

Sprite* sprites = NULL;
Offset* offsets = NULL;
SpriteMap* spritemaps = NULL;

void add_offset(const char* name, int x, int y) {
  struct Offset *o;
  o = malloc(sizeof(Offset));
  if (o == NULL) {
    exit(-1);
  }
  strcpy(o->name, name);
  o->x = x;
  o->y = y;
  HASH_ADD_STR(offsets, name, o);
}

Offset* get_offset(const char* name) {
  struct Offset *o;

  HASH_FIND_STR(offsets, name, o);
  if (o) {
    return o;
  } else {
    return NULL;
  }
}

void add_sprite(const char* name, SDL_Texture* image) {
  struct Sprite *s;
  s = malloc(sizeof(Sprite));
  if (s == NULL) {
    exit(-1);
  }
  strcpy(s->name, name);
  s->image = image;
  HASH_ADD_STR(sprites, name, s);
}

SDL_Texture* get_sprite(const char* name) {
  struct Sprite *s;
  
  HASH_FIND_STR(sprites, name, s);
  if (s) {
    return s->image;
  } else {
    return NULL;
  }
}

SDL_Surface* load_image(const char* filename) {
  SDL_Surface* image = IMG_Load(filename);
  if (!image) {
    return NULL;
  }
  return image;
}

void load_spritesheet(SDL_Renderer* rend,
		      const char* filename,
		      const char* names[],
		      int xs[],
		      int ys[],
		      int ws[],
		      int hs[],
		      int count) {

  SDL_Surface* spritesheet = load_image(filename);
  if (!spritesheet)
    {
      printf("Could not load %s\n", filename);
      return;
    }

  SDL_SetColorKey(spritesheet, SDL_TRUE, SDL_MapRGB(spritesheet->format, 1, 255, 1));
  for (int i = 0; i < count; i++) {
    if (ws[i] == 0) {
      ws[i] = 1;
    }
    if (hs[i] == 0) {
      hs[i] = 1;
    }
    
    SDL_Surface* sprite = SDL_CreateRGBSurfaceWithFormat(0, ws[i], hs[i], 32, spritesheet->format->format);
    SDL_Rect src;
    SDL_Rect dst;
    src.x = xs[i];
    src.y = ys[i];
    src.w = ws[i];
    src.h = hs[i];
    dst.x = 0;
    dst.y = 0;
    dst.w = sprite->w;
    dst.h = sprite->h;
    
    SDL_BlitSurface(spritesheet, &src, sprite, &dst);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rend, sprite);

    add_sprite(names[i], texture);

    SDL_FreeSurface(sprite);
  }

  SDL_FreeSurface(spritesheet);
}
