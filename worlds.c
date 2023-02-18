# include "actors.h"
# include "worlds.h"
# include "sprites.h"
# include "utlist.h"
# include "uthash.h"

World* worlds = NULL;

void add_world(const char* name,
	       char* background,
	       int x_lock,
	       int y_lock) {
  struct World *w;
  w = malloc(sizeof(World));
  if (!w) {
    exit(-1);
  }
  w->actors = NULL;
  strcpy(w->name, name);
  strcpy(w->background, background);
  if (x_lock)
    w->x_lock = x_lock;
  if (y_lock)
    w->y_lock = y_lock;
  w->background_x_scroll = 0;
  w->background_y_scroll = 0;
  w->flagged_for_update = 1;

  HASH_ADD_STR(worlds, name, w);
}

World* get_world(const char* name) {
  struct World *w;
  HASH_FIND_STR(worlds, name, w);
  if (w) {
    return w;
  } else {
    return NULL;
  }
}

void add_actor_to_world(const char* worldkey, const char* actorname) {
  struct World *w;
  w = get_world(worldkey);
  if (!w) {
    printf("World %s not found when adding actor %s\n", worldkey, actorname);
    return;
  }
  struct ActorEntry *ae;
  ae = malloc(sizeof(ActorEntry));
  strcpy(ae->actorKey, actorname);

  DL_APPEND(w->actors, ae);
}

void update_world(char* worldKey) {
  struct World *w;
  w = get_world(worldKey);
  struct ActorEntry *ae, *tmp;
  DL_FOREACH_SAFE(w->actors, ae, tmp) {
    update_actor(ae->actorKey);
  }
}

void _draw_background(World* world, SDL_Renderer* rend) {
  Sprite *background;
  background = get_sprite(world->background);
  if (!background) {
    return;
  }
  SDL_Rect dest;
  SDL_Rect src;
  src.x = 0;
  src.y = 0;
  dest.x = 0;
  dest.y = 0;
  SDL_QueryTexture(background->image, NULL, NULL, &dest.w, &dest.h);
  SDL_QueryTexture(background->image, NULL, NULL, &src.w, &src.h);
  int w, h;
  SDL_GetRendererOutputSize(rend, &w, &h);

  for (int y = 0; y < h/dest.h+2; y++) {
    dest.y = y*dest.h;
    for (int x = 0; x < w/dest.w+2; x++) {
      dest.x = x*dest.w;
      // TODO: offset by frame background offset value
      SDL_RenderCopy(rend, background->image, &src, &dest);
    }
  }
}

void draw_world(World* world, SDL_Renderer* rend) {
  _draw_background(world, rend);

  struct ActorEntry *ae;
  DL_FOREACH(world->actors, ae) {
    Actor* a;
    a = get_actor(ae->actorKey);
    if (!a) continue;

    Sprite *s;
    s = get_sprite_for_actor(ae->actorKey);
    if (!s) continue;
    SDL_Rect dest;
    SDL_Rect src;
    dest.x = a->ECB->x + s->offx;
    dest.y = a->ECB->y + s->offy;
    src.x = 0;
    src.y = 0;

    SDL_QueryTexture(s->image, NULL, NULL, &dest.w, &dest.h);
    SDL_QueryTexture(s->image, NULL, NULL, &src.w, &src.h);
    
    SDL_RenderCopy(rend, s->image, &src, &dest);
  }
};
