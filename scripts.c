#include "utlist.h"
#include "uthash.h"
#include "scripts.h"
#include <SDL2/SDL.h>

Script* scripts = NULL;
ScriptMap* scriptmaps = NULL;

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
