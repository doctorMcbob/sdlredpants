# include <SDL2/SDL.h>
# include <stdio.h>
# include <string.h>
# include "uthash.h"
# include "actors.h"
# include "frames.h"
# include "scripts.h"

Actor* actors = NULL;
Actor* templates = NULL;

Actor* get_actor(const char* name) {
  struct Actor *a;
  HASH_FIND_STR(actors, name, a);
  if (a) {
    return a;
  } else {
    return NULL;
  }
}

void add_actor(const char* name,
	       int x,
	       int y,
	       int w,
	       int h,
	       int x_vel,
	       int y_vel,
	       char* hurtboxkey,
	       char* hitboxkey,
	       char* scriptmapkey,
	       char* spritemapkey,
	       char* img,
	       char* inputKey,
	       char* state,
	       int frame,
	       int direction,
	       int rotation,
	       int platform,
	       int tangible,
	       int physics,
	       int updated) {
  struct Actor *a;
  a = malloc(sizeof(Actor));
  if (!a) {
    exit(-1);
  }
  a->attributes = NULL;
  SDL_Rect *ecb;
  ecb = malloc(sizeof(SDL_Rect));
  if (!ecb) {
    exit(-1);
  }
  ecb->x = x;
  ecb->y = y;
  ecb->w = w;
  ecb->h = h;
  a->ECB = ecb;
  strcpy(a->name, name);
  if (x_vel)
    a->x_vel = x_vel;
  else
    a->x_vel = 0;
  if (y_vel)
    a->y_vel = y_vel;
  else
    a->y_vel = 0;
  if (hurtboxkey)
    strcpy(a->hurtboxkey, hurtboxkey);
  if (hitboxkey)
    strcpy(a->hitboxkey, hitboxkey);
  if (spritemapkey)
    strcpy(a->spritemapkey, spritemapkey);
  if (scriptmapkey)
    strcpy(a->scriptmapkey, scriptmapkey);

  if (img)
    strcpy(a->img, img);

  if (inputKey)
    strcpy(a->_input_name, inputKey);

  if (state)
    strcpy(a->state, state);
  else
    strcpy(a->state, "START");

  if (frame)
    a->frame = frame;
  else
    a->frame = 0;

  if (direction)
    a->direction = direction;
  else
    a->direction = -1;

  if (rotation)
    a->rotation = rotation;
  else
    a->rotation = 0;

  if (platform)
    a->platform = platform;
  else
    a->platform = 0;

  if (tangible)
    a->tangible = tangible;
  else
    a->tangible = 0;

  if (physics)
    a->physics = physics;
  else
    a->physics = 0;

  if (updated)
    a->updated = updated;
  else
    a->updated = 0;

  HASH_ADD_STR(actors, name, a);
}

void copy_actor(Actor* copy,  Actor *a) {
  a->ECB->x = copy->ECB->x;
  a->ECB->y = copy->ECB->y;
  a->ECB->w = copy->ECB->w;
  a->ECB->h = copy->ECB->h;
  strcpy(a->name, copy->name);
  a->x_vel = copy->x_vel;
  a->y_vel = copy->y_vel;
  strcpy(a->hurtboxkey, copy->hurtboxkey);
  strcpy(a->hitboxkey, copy->hitboxkey);
  strcpy(a->spritemapkey, copy->spritemapkey);
  strcpy(a->scriptmapkey, copy->scriptmapkey);
  strcpy(a->img, copy->img);
  strcpy(a->_input_name, copy->_input_name);
  strcpy(a->state, copy->state);
  a->frame = copy->frame;
  a->direction = copy->direction;
  a->rotation = copy->rotation;
  a->platform = copy->platform;
  a->tangible = copy->tangible;
  a->physics = copy->physics;
  a->updated = copy->updated;
}

void add_template(Actor* copy) {
  struct Actor *a;
  a = malloc(sizeof(Actor));
  if (!a) {
    exit(-1);
  }
  SDL_Rect *ecb;
  ecb = malloc(sizeof(SDL_Rect));
  if (!ecb) {
    exit(-1);
  }
  a->ECB = ecb;
  copy_actor(copy, a);
  
  HASH_ADD_STR(templates, name, a);
}

void add_actor_from_templatekey(char* templateKey) {
  struct Actor *copy;
  HASH_FIND_STR(templates, templateKey, copy);
  struct Actor *a;
  a = malloc(sizeof(Actor));
  if (!a) {
    exit(-1);
  }
  SDL_Rect *ecb;
  ecb = malloc(sizeof(SDL_Rect));
  if (!ecb) {
    exit(-1);
  }
  a->ECB = ecb;
  copy_actor(copy, a);
  
  HASH_ADD_STR(actors, name, a);
}

void add_template_from_actorkey(char* actorKey) {
  struct Actor *copy;
  HASH_FIND_STR(actors, actorKey, copy);
  struct Actor *a;
  a = malloc(sizeof(Actor));
  if (!a) {
    exit(-1);
  }
  SDL_Rect *ecb;
  ecb = malloc(sizeof(SDL_Rect));
  if (!ecb) {
    exit(-1);
  }
  a->ECB = ecb;
  copy_actor(copy, a);
  
  HASH_ADD_STR(templates, name, a);
}

int update_actor(char* actorKey, char* worldKey) {
  Actor *actor = get_actor(actorKey);
  if (!actor) return 0;
  int scriptKey = get_script_for_actor(actor);
  if (scriptKey != -1) {
    return resolve_script(scriptKey, worldKey, actorKey, NULL);
  }
  return 0;
}

int get_script_for_actor(Actor* actor) {
  ScriptMap *sm = get_script_map(actor->scriptmapkey);
  if (!sm) return -1;
  ScriptMapEntry *best, *sme;
  best = NULL;
    DL_FOREACH(sm->entries, sme) {
    if (strcmp(actor->state, sme->state) != 0) continue;
    if (actor->frame < sme->frame) continue;
    if (best)
      if (sme->frame > best->frame) continue;
    best = sme;
  }
  if (!best) {
    return -1;
  }
  return best->scriptKey;
}

Sprite* get_sprite_for_actor(Actor* actor) {
  if (actor->platform) {
    return NULL;
  }
  struct SpriteMap *sm;
  sm = get_sprite_map(actor->spritemapkey);
  if (!sm) return NULL;
  SpriteMapEntry *best, *sme;
  best = NULL;
  DL_FOREACH(sm->entries, sme) {
    if (strcmp(actor->state, sme->state) != 0) continue;
    if (actor->frame < sme->frame) continue;
    if (best)
      if (sme->frame > best->frame) continue;
    best = sme;
  }
  if (!best) {
    return NULL;
  }
  return get_sprite(best->spriteKey);
}

void _draw_platform(SDL_Renderer* rend, Actor* actor) {
  // currently the grossest
  struct SpriteMap *sm;
  sm = get_sprite_map(actor->spritemapkey);
  if (!sm) return;

  char key00[32],key01[32],key02[32],
       key10[32],key11[32],key12[32],
       key20[32],key21[32],key22[32];

  sprintf(key00, "%s%i%i", actor->state, 0, 0);
  sprintf(key01, "%s%i%i", actor->state, 0, 1);
  sprintf(key02, "%s%i%i", actor->state, 0, 2);
  sprintf(key10, "%s%i%i", actor->state, 1, 0);
  sprintf(key11, "%s%i%i", actor->state, 1, 1);
  sprintf(key12, "%s%i%i", actor->state, 1, 2);
  sprintf(key20, "%s%i%i", actor->state, 2, 0);
  sprintf(key21, "%s%i%i", actor->state, 2, 1);
  sprintf(key22, "%s%i%i", actor->state, 2, 2);
  
  Sprite *s00, *s01, *s02,
         *s10, *s11, *s12,
         *s20, *s21, *s22;
  
  SpriteMapEntry *sme;
  DL_FOREACH(sm->entries, sme) {
    if (strcmp(sme->state, key00) == 0) {
      s00 = get_sprite(sme->spriteKey);
      continue;
    }
    if (strcmp(sme->state, key01) == 0) {
      s01 = get_sprite(sme->spriteKey);
      continue;
    }
    if (strcmp(sme->state, key02) == 0) {
      s02 = get_sprite(sme->spriteKey);
      continue;
    }
    if (strcmp(sme->state, key10) == 0) {
      s10 = get_sprite(sme->spriteKey);
      continue;
    }
    if (strcmp(sme->state, key11) == 0) {
      s11 = get_sprite(sme->spriteKey);
      continue;
    }
    if (strcmp(sme->state, key12) == 0) {
      s12 = get_sprite(sme->spriteKey);
      continue;
    }
    if (strcmp(sme->state, key20) == 0) {
      s20 = get_sprite(sme->spriteKey);
      continue;
    }
    if (strcmp(sme->state, key21) == 0) {
      s21 = get_sprite(sme->spriteKey);
      continue;
    }
    if (strcmp(sme->state, key22) == 0) {
      s22 = get_sprite(sme->spriteKey);
    }
  }

  for (int y=0; y < actor->ECB->h / 32; y++) {
    for (int x=0; x < actor->ECB->w / 32; x++) {
      Sprite* img = NULL;
      if      (x == 0                  && y == 0                 ) img = s00;
      else if (x == actor->ECB->w/32-1 && y == 0                 ) img = s02;
      else if (x == 0                  && y == actor->ECB->h/32-1) img = s20;
      else if (x == actor->ECB->w/32-1 && y == actor->ECB->h/32-1) img = s22;
      else if (x == 0                                            ) img = s10;
      else if (x == actor->ECB->w/32-1                           ) img = s12;
      else if (                           y == 0                 ) img = s01;
      else if (                           y == actor->ECB->h/32-1) img = s21;
      else img = s11;

      if (img == NULL) {
	continue;
      }
      SDL_Rect dest, src;
      dest.x = actor->ECB->x + x*32;
      dest.y = actor->ECB->y + y*32;
      src.x = 0;
      src.y = 0;
      
      SDL_QueryTexture(img->image, NULL, NULL, &dest.w, &dest.h);
      SDL_QueryTexture(img->image, NULL, NULL, &src.w, &src.h);
      
      SDL_RenderCopy(rend, img->image, &src, &dest);
    }
  }
}


void draw_actor(SDL_Renderer* rend, Actor* actor, const char* frameKey) {
  if (actor->platform) {
    return _draw_platform(rend, actor);
  }
  Sprite *s;
  s = get_sprite_for_actor(actor);
  if (s == NULL) return;
  Frame *f;
  f = get_frame(frameKey);
  if (f == NULL) return;
  
  SDL_Rect dest, src;
  dest.x = actor->ECB->x + s->offx - f->scroll_x;
  dest.y = actor->ECB->y + s->offy - f->scroll_y;
  src.x = 0;
  src.y = 0;
  
  SDL_QueryTexture(s->image, NULL, NULL, &dest.w, &dest.h);
  src.w = dest.w;
  src.h = dest.h;
  
  SDL_RenderCopy(rend, s->image, &src, &dest);
}

