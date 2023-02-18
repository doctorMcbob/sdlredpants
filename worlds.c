# include "actors.h"
# include "worlds.h"
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
  printf("Added actor %s to world %s\n", actorname, worldkey);
}

