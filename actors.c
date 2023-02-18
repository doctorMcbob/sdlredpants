# include <SDL2/SDL.h>
# include <stdio.h>
# include <string.h>
# include "uthash.h"
# include "actors.h"

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

