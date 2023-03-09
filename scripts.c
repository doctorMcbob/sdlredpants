#include "utlist.h"
#include "uthash.h"
#include "scripts.h"
#include "actors.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>

Script* scripts = NULL;
ScriptMap* scriptmaps = NULL;

void free_SyntaxNode(SyntaxNode* del) {
  switch (del->type) {
  case STRING:
    free(del->data.s);
    break;
  case LIST:
    {
      SyntaxNode *sn, *tmp; 
      DL_FOREACH_SAFE(del->data.list, sn, tmp) {
	DL_DELETE(del->data.list, sn);
	free_SyntaxNode(sn);
      }
      break;
    }
  }
  free(del);
}

SyntaxNode* copy_SyntaxNode(SyntaxNode* orig) {
  SyntaxNode *copy = malloc(sizeof(SyntaxNode));
  copy->type = orig->type;
  switch (orig->type) {
  case STRING: {
    copy->data.s = malloc(sizeof(orig->data.s));
    strcpy(copy->data.s, orig->data.s);
    break;
  }
  case LIST: {
    SyntaxNode *sn;
    copy->data.list = NULL;
    DL_FOREACH(orig->data.list, sn) {
      SyntaxNode* cpy = copy_SyntaxNode(sn);
      DL_APPEND(copy->data.list, cpy);
    }
    break;
  }
  }
  return copy;
}

void add_script(int scriptKey) {
  struct Script* sc;
  sc = malloc(sizeof(Script));
  if (sc == NULL) {
    exit(-1);
  }
  sc->scriptKey = scriptKey;
  sc->statements = NULL;
  HASH_ADD_INT(scripts, scriptKey, sc);
};

Script* get_script(int scriptKey) {
  struct Script* sc;
  HASH_FIND_INT(scripts, &scriptKey, sc);
  if (sc)
    return sc;
  return NULL;
}

void add_statement_to_script(int scriptKey, Statement* statement) {
  struct Script* sc;
  sc = get_script(scriptKey);

  DL_APPEND(sc->statements, statement);
};

Statement* new_statement(int verb) {
  struct Statement* s;
  s = malloc(sizeof(Statement));
  if (s == NULL) {
    exit(-1);
  }
  s->params = NULL;
  s->script = NULL;
  s->verb = verb;
  return s;
}

SyntaxNode* new_syntax_node(int type) {
  struct SyntaxNode* sn;
  sn = malloc(sizeof(SyntaxNode));
  if (sn == NULL) {
    exit(-1);
  }
  sn->type = type;
  return sn;
};

SyntaxNode* _get_last(SyntaxNode *head) {
  SyntaxNode *curr = head;
  while (curr != NULL && curr->next != NULL)
    curr = curr->next;
  return curr;
}

void add_node_to_statement(Statement* statement, SyntaxNode* node) {
  DL_APPEND(statement->script, node);
}

void add_script_map(const char* name) {
  struct ScriptMap* scm;
  scm = malloc(sizeof(ScriptMap));
  if (scm == NULL) {
    exit(-1);
  }
  strcpy(scm->name, name);
  scm->entries = NULL;
  HASH_ADD_STR(scriptmaps, name, scm);
}

ScriptMap* get_script_map(const char* name) {
  struct ScriptMap* scm;
  HASH_FIND_STR(scriptmaps, name, scm);
  if (scm) 
    return scm;
  return NULL;
}

void add_script_to_script_map(const char* name, char* state, int frame, int scriptKey) {
  struct ScriptMap* scm = get_script_map(name);
  if (!scm) {
    printf("No ScriptMap %s\n", name);
    return;
  }
  struct ScriptMapEntry* sme;
  sme = malloc(sizeof(ScriptMapEntry));
  strcpy(sme->state, state);
  sme->frame = frame;
  sme->scriptKey = scriptKey;

  DL_APPEND(scm->entries, sme);
}

void resolve_script(int scriptKey,
		    char* worldKey,
		    char* selfActorKey,
		    char* relatedActorKey) {
  Script* script = get_script(scriptKey);
  Statement* statement;
  
  DL_FOREACH(script->statements, statement) {
    clean_statement(statement);
    evaluate_literals(statement, worldKey, selfActorKey, relatedActorKey);
    resolve_operators(statement, worldKey, selfActorKey, relatedActorKey);
    resolve_verb(statement, worldKey, selfActorKey, relatedActorKey);
  }
}

void clean_statement(Statement* statement) {
  SyntaxNode *sn, *tmp;
  DL_FOREACH_SAFE(statement->params, sn, tmp) {
    DL_DELETE(statement->params, sn);
    free(sn);
  }

  statement->params = NULL;
}

void evaluate_literals(Statement* statement,
		       char* worldKey,
		       char* selfActorKey,
		       char* relatedActorKey) {
  SyntaxNode *sn, *tmp;
  DL_FOREACH_SAFE(statement->script, sn, tmp) {
    SyntaxNode *new;
    switch (sn->type) {
    case NONE:
    case FLOAT:
    case INT:
    case STRING:
    case OPERATOR:
      new = malloc(sizeof(SyntaxNode));
      new->type = sn->type;
      new->data = sn->data;
      DL_APPEND(statement->params, new);
      break;
    case DOT: {
      SyntaxNode* parameter = _get_last(statement->params);
      if (parameter == NULL || sn->next == NULL) {
	printf("actor %s: Missing parameter for dot notation on verb %i\n", selfActorKey, statement->verb);
	break;
      }
      if (parameter->type != STRING) {
	printf("actor %s: Cannot use dot notation on type %i\n", selfActorKey, parameter->type);
	break;
      }
      if (sn->next->type != STRING) {
	printf("actor %s: Cannot use dot notation with type %i\n", selfActorKey, sn->next->type);
	break;
      }
      char *actorKey = parameter->data.s;
      if (strcmp(actorKey, "self") == 0) actorKey = selfActorKey;
      else if (strcmp(actorKey, "related") == 0) actorKey= relatedActorKey;
      Actor *actor = get_actor(actorKey);
      if (!actor) {
	printf("Could not find actor \n");
	break;
      }
      // check special (top level actor attributes, not in hash)
      // x, y, w, h,
      // left, top, right, bottom,
      // name, state, frame,
      // x_vel, y_vel, direction, rotation,
      // platform, tangible, physics
      /**
       Quick note because i know ill forget, 
       right now: self.frame is (self) (.) (frame)
       0) (self) gets added to params
       1) (.) removes (self) from params, and checks (frame) of self
       2) (value of self.frame) gets put on params
       3 TODO !!) skip over the laready leveraged (frame)
       this needs to happen in each of these cases below as well as at the bottom of case DOT: 
      */
      
      if (strcmp(sn->next->data.s, "x") == 0) {
	new = new_syntax_node(INT);
	new->data.i = actor->ECB->x;
	DL_DELETE(statement->params, parameter);
	free(parameter);
	DL_APPEND(statement->params, new);
	break;
      }
      if (strcmp(sn->next->data.s, "y") == 0) {
	new = new_syntax_node(INT);
	new->data.i = actor->ECB->y;
	DL_DELETE(statement->params, parameter);
	free(parameter);
	DL_APPEND(statement->params, new);
	break;
      }
      if (strcmp(sn->next->data.s, "w") == 0) {
	new = new_syntax_node(INT);
	new->data.i = actor->ECB->w;
	DL_DELETE(statement->params, parameter);
	free(parameter);
	DL_APPEND(statement->params, new);
	break;
      }
      if (strcmp(sn->next->data.s, "h") == 0) {
	new = new_syntax_node(INT);
	new->data.i = actor->ECB->h;
	DL_DELETE(statement->params, parameter);
	free(parameter);
	DL_APPEND(statement->params, new);
	break;
      }
      
      Attribute *attribute = NULL;
      HASH_FIND_STR(actor->attributes, sn->next->data.s, attribute);
      if (attribute == NULL) {
	printf("actor %s: could not find attribute of %s, %s\n", selfActorKey, parameter->data.s, sn->next->data.s);
	break;
      }
      new = copy_SyntaxNode(attribute->value);
      DL_DELETE(statement->params, parameter);
      free(parameter);
      DL_APPEND(statement->params, new);
      break;
    }
    case QRAND:
      new = malloc(sizeof(SyntaxNode));
      new->type = INT;
      new->data.i = rand() % 2;
      DL_APPEND(statement->params, new);
      break;
    case QWORLD:
      // put new SyntaxNode with type STRING with the worldKey
      break;
    case QSONG:
      // put new SyntaxNode with type STRING with the key for the active song
      break;
    case QCOLLIDE:
      // check if the actorKey is colliding with any other tangible actors int he worldKey
      // put new SyntaxNode with type INT 0 or 1
      break;
    case LIST:
      // put new SyntaxNode with type LIST onto params, setting data.list to NULL for the head of a DL
      break;
    case INP_A:
      // put new SyntaxNode with type INT onto params, setting data.i to actorKey->_input_state A
      break;
    case INP_B:
      // put new SyntaxNode with type INT onto params, setting data.i to actorKey->_input_state B
      break;
    case INP_X:
      // put new SyntaxNode with type INT onto params, setting data.i to actorKey->_input_state X
      break;
    case INP_Y:
      // put new SyntaxNode with type INT onto params, setting data.i to actorKey->_input_state Y
      break;
    case INP_LEFT:
      // put new SyntaxNode with type INT onto params, setting data.i to actorKey->_input_state LEFT
      break;
    case INP_UP:
      // put new SyntaxNode with type INT onto params, setting data.i to actorKey->_input_state UP
      break;
    case INP_RIGHT:
      // put new SyntaxNode with type INT onto params, setting data.i to actorKey->_input_state RIGHT
      break;
    case INP_DOWN:
      // put new SyntaxNode with type INT onto params, setting data.i to actorKey->_input_state DOWN
      break;
    case INP_START:
      // put new SyntaxNode with type INT onto params, setting data.i to actorKey->_input_state START
      break;
    case INP_EVENTS:
      // create a new SyntaxNode DL and fill it with SyntaxNodes with type STRING for each event we have in actorKey->_input_state EVENTS
      break;
    }
  }
}

void normalize_left_right(SyntaxNode* left, SyntaxNode* right) {
  if (left->type == INT && right->type == STRING) {
    int i = left->data.i;
    int len = snprintf(NULL, 0, "%i", i);
    left->data.s = malloc(len+1);
    left->type = STRING;
    sprintf(left->data.s, "%i", i);
  }

  if (left->type == FLOAT && right->type == STRING) {
    float f = left->data.f;
    left->type = STRING;
    sprintf(left->data.s, "%f", f);
  }

  if (left->type == STRING && right->type == INT) {
    int i = right->data.i;
    right->type = STRING;
    int len = snprintf(NULL, 0, "%i", i);
    right->data.s = malloc(len+1);
    sprintf(right->data.s, "%i", i);
  }

  if (left->type == STRING && right->type == FLOAT) {
    float f = right->data.f;
    right->type = STRING;
    sprintf(right->data.s, "%f", f);
  }
}

void pop_nbrs(SyntaxNode* head, SyntaxNode* elm) {
  SyntaxNode *hold;
  hold = elm->next;
  DL_DELETE(head, hold);
  free(hold);
  hold = elm->prev;
  DL_DELETE(head, hold);
  free(hold);
}

void resolve_operators(Statement* statement,
		       char* worldKey,
		       char* selfActorKey,
		       char* relatedActorKey) {
  SyntaxNode *sn, *tmp;
  DL_FOREACH_SAFE(statement->params, sn, tmp) {
    if (sn->type != OPERATOR) continue;
    switch (sn->data.i) {
    case PLUS:
      if (sn->prev == NULL || sn->next == NULL) break;
      normalize_left_right(sn->prev, sn->next);
      if (sn->prev->type == STRING && sn->next->type == STRING) {
	int len = strlen(sn->prev->data.s) + strlen(sn->next->data.s) + 1;
	char* combined_str = (char*)malloc(len * sizeof(char));
	snprintf(combined_str, len, "%s%s", sn->prev->data.s, sn->next->data.s);
	sn->type = STRING;
	sn->data.s = combined_str;
      } else if (sn->prev->type == INT && sn->next->type == INT) {
	sn->type = INT;
	sn->data.i = sn->prev->data.i + sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->type = FLOAT;
	sn->data.f = sn->prev->data.i + sn->next->data.f;
      } else if (sn->prev->type == FLOAT && sn->next->type == INT) {
	sn->type = FLOAT;
	sn->data.f = sn->prev->data.f + sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->type = FLOAT;
	sn->data.f = sn->prev->data.f + sn->next->data.f;
      } else {
	printf("Could not + types: %i, %i\n", sn->prev->type, sn->next->type);
      }
      pop_nbrs(statement->params, sn);
      break;
    case MINUS:
      if (sn->prev == NULL || sn->next == NULL) break;
      if (sn->prev->type == INT && sn->next->type == INT) {
	sn->type = INT;
	sn->data.i = sn->prev->data.i - sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->type = FLOAT;
	sn->data.f = sn->prev->data.i - sn->next->data.f;
      } else if (sn->prev->type == FLOAT && sn->next->type == INT) {
	sn->type = FLOAT;
	sn->data.f = sn->prev->data.f - sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->type = FLOAT;
	sn->data.f = sn->prev->data.f - sn->next->data.f;
      } else {
	printf("Could not - types: %i, %i\n", sn->prev->type, sn->next->type);
      }
      pop_nbrs(statement->params, sn);
      break;
    case MULT:
      if (sn->prev == NULL || sn->next == NULL) break;
      if (sn->prev->type == INT && sn->next->type == INT) {
	sn->type = INT;
	sn->data.i = sn->prev->data.i * sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->type = FLOAT;
	sn->data.f = sn->prev->data.i * sn->next->data.f;
      } else if (sn->prev->type == FLOAT && sn->next->type == INT) {
	sn->type = FLOAT;
	sn->data.f = sn->prev->data.f * sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->type = FLOAT;
	sn->data.f = sn->prev->data.f * sn->next->data.f;
      } else {
	printf("Could not * types: %i, %i\n", sn->prev->type, sn->next->type);
      }
      pop_nbrs(statement->params, sn);
      break;
    case FLOORDIV:
      if (sn->prev == NULL || sn->next == NULL) break;
      sn->type = INT;
      if (sn->prev->type == INT && sn->next->type == INT) {
	sn->data.i = sn->prev->data.i / sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->data.i = sn->prev->data.i / (int)sn->next->data.f;
      } else if (sn->prev->type == FLOAT && sn->next->type == INT) {
	sn->data.i = (int)sn->prev->data.f / sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->data.i = (int)sn->prev->data.f / (int)sn->next->data.f;
      } else {
	printf("Could not // types: %i, %i\n", sn->prev->type, sn->next->type);
      }
      pop_nbrs(statement->params, sn);
      break;
    case FLOATDIV:
      if (sn->prev == NULL || sn->next == NULL) break;
      sn->type = FLOAT;
      if (sn->prev->type == INT && sn->next->type == INT) {
	sn->data.f = sn->prev->data.i / (float)sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->data.f = sn->prev->data.i / sn->next->data.f;
      } else if (sn->prev->type == FLOAT && sn->next->type == INT) {
	sn->data.f = sn->prev->data.f / sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->data.f = sn->prev->data.f / sn->next->data.f;
      } else {
	printf("Could not / types: %i, %i\n", sn->prev->type, sn->next->type);
      }
      pop_nbrs(statement->params, sn);
      break;
    case MOD:
      if (sn->prev == NULL || sn->next == NULL) break;
      sn->type = INT;
      if (sn->prev->type == INT && sn->next->type == INT) {
	sn->data.i = sn->prev->data.i % sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->data.f = sn->prev->data.i % (int)sn->next->data.f;
      } else if (sn->prev->type == FLOAT && sn->next->type == INT) {
	sn->data.f = (int)sn->prev->data.f % sn->next->data.i;
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->data.f = (int)sn->prev->data.f % (int)sn->next->data.f;
      } else {
	printf("Could not %% types: %i, %i\n", sn->prev->type, sn->next->type);
      }
      pop_nbrs(statement->params, sn);
      break;
    case POW:
      if (sn->prev == NULL || sn->next == NULL) break;
      if (sn->prev->type == INT && sn->next->type == INT) {
	sn->type = INT;
	sn->data.i = pow(sn->prev->data.i,  sn->next->data.i);
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->type = FLOAT;
	sn->data.f = pow(sn->prev->data.i, sn->next->data.f);
      } else if (sn->prev->type == FLOAT && sn->next->type == INT) {
	sn->type = FLOAT;
	sn->data.f = pow(sn->prev->data.f, sn->next->data.i);
      } else if (sn->prev->type == INT && sn->next->type == FLOAT) {
	sn->type = FLOAT;
	sn->data.f = pow(sn->prev->data.f, sn->next->data.f);
      } else {
	printf("Could not ** types: %i, %i\n", sn->prev->type, sn->next->type);
      }
      pop_nbrs(statement->params, sn);
      break;
    case EQUALS:
      break;
    case LESSTHAN:
      break;
    case MORETHAN:
      break;
    case NOTEQUAL:
      break;
    case AND:
      break;
    case OR:
      break;
    case NOT:
      break;
    case NOR:
      break;
    case IN:
      break;
    case AT:
      break;
    case CASTINT:
      break;
    case CASTSTR:
      break;
    case MIN:
      break;
    case MAX:
      break;
    case LEN:
      break;
    case COUNTOF:
      break;
    case EXISTS:
      break;
    case HASFRAME:
      break;
    case CHOICEOF:
      break;
    case ISFRAME:
      break;
    case ISINPUTSTATE:
      break;
    case ABS:
      break;
    case RANGE:
      break;
    }
  }
}

void resolve_verb(Statement* statement,
		  char* worldKey,
		  char* selfActorKey,
		  char* relatedActorKey) {
  switch (statement->verb) {
  case QUIT:
    break;
  case GOODBYE:
    break;
  case BREAK:
    break; // nice
  case RESET:
    break;
  case SET:
    break;
  case REASSIGN:
    break;
  case IF:
    break;
  case EXEC:
    break;
  case BACK:
    break;
  case FRONT:
    break;
  case IMG:
    break;
  case ACTIVATE:
    break;
  case DEACTIVATE:
    break;
  case KILLFRAME:
    break;
  case MAKEFRAME:
    break;
  case FOCUS:
    break;
  case SCROLLBOUND:
    break;
  case VIEW:
    break;
  case MOVE:
    break;
  case PLACE:
    break;
  case TAKE:
    break;
  case TAKEALL:
    break;
  case REBRAND:
    break;
  case REMOVE:
    break;
  case ADD:
    break;
  case HITBOXES:
    break;
  case HURTBOXES:
    break;
  case CREATE:
    break;
  case UPDATE:
    break;
  case SFX:
    break;
  case SONG:
    break;
  case SFXOFF:
    break;
  case SONGOFF:
    break;
  case OFFSETBGSCROLLX:
    break;
  case OFFSETBGSCROLLY:
    break;
  case FOR:
    break;
  case PRINT:
    break;
  case UPDATE_STICKS:
    break;
  }
}

