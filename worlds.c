typedef struct World {
  char name[32];
  char background[32];
  // need to implement doubly linked list :E
  char actors[32][32];
  int x_lock;
  int y_lock;
  int bacground_x_scroll;
  int bacground_y_scroll;
  int flagged_for_update;
} World;
