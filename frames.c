#include "uthash.h"
#include "worlds.h"
#include "actors.h"
#include "frames.h"
# include <SDL2/SDL.h>

Frame* frames = NULL;

void add_frame(const char* name, World* world, Actor* focus, int x, int y, int w, int h) {
  struct Frame *f;
  f = malloc(sizeof(Frame));
  if (f == NULL) {
    exit(-1);
  }
  strcpy(f->name, name);
  SDL_Rect* rect;
  rect = malloc(sizeof(SDL_Rect));
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
  f->rect = rect;
  f->world = world;
  f->focus = focus;
  HASH_ADD_STR(frames, name, f);
}

Frame* get_frame(const char* name){
  struct Frame* f;
  HASH_FIND_STR(frames, name, f);
  if (f) {
    return f;
  } else {
    return NULL;
  }
}

SDL_Rect* scrolled(SDL_Rect* rect, Frame* frame) {
  SDL_Rect scrolled;
  scrolled.x = rect->x - frame->scroll_x;
  scrolled.y = rect->y - frame->scroll_y;
  scrolled.w = rect->w;
  scrolled.h = rect->h;
  SDL_Rect* r = &scrolled;
  return r;
}

int in_frame(const char* frameKey, Actor* actor) {
  Frame* frame = get_frame(frameKey);
  if (!frame) return 0;

  if (SDL_HasIntersection(frame->rect, scrolled(actor->ECB, frame)))
    return 1;
  else
    return 0;
}

void draw_frame(SDL_Renderer* rend, const char* name) {
  struct Frame* f;
  f = get_frame(name);
  Uint32 format;
  SDL_Texture* render_target = SDL_GetRenderTarget(rend);
  SDL_QueryTexture(render_target, &format, NULL, NULL, NULL);
  SDL_Texture* frame_buffer = SDL_CreateTexture(rend, format, SDL_TEXTUREACCESS_TARGET, f->rect->w, f->rect->h);

  SDL_SetRenderTarget(rend, frame_buffer);
  
  draw_world(f->world, rend, name);

  SDL_SetRenderTarget(rend, render_target);

  SDL_Rect src = {0, 0, f->rect->w, f->rect->h};
  SDL_RenderCopy(rend, frame_buffer, &src, f->rect);
  
  SDL_DestroyTexture(frame_buffer);
}
