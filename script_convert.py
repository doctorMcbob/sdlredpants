from SCRIPTS import SCRIPTS

LITERAL_KEYWORDS = {
    "None": 0,
    "RAND?": 6,
    "WORLD?": 7,
    "SONG?": 8,
    "COLLIDE?": 9,
    "[]": 10,
    "inpA": 11,
    "inpB": 12,
    "inpX": 13,
    "inpY": 14,
    "inpLEFT": 15,
    "inpUP": 16,
    "inpRIGHT": 17,
    "inpDOWN": 18,
    "inpSTART": 19,
    "inpEVENTS": 20,
}

OPERATORS = {
    "+": 0,
    "-": 1,
    "*": 2,
    "//": 4,
    "////": 3,
    "%": 5,
    "**": 6,
    "==": 7,
    "<": 8,
    ">": 9,
    "<=": 10,
    ">=": 11,
    "!=": 12,
    "and": 13,
    "or": 14,
    "not": 15,
    "nor": 16,
    "in": 17,
    "at": 18,
    "int": 19,
    "str": 20,
    "min": 21,
    "max": 22,
    "len": 23,
    "countof": 24,
    "exists": 25,
    "hasframe": 26,
    "choiceof": 27,
    "isframe": 28,
    "isinputstate": 29,
    "abs": 30,
    "range": 31,
}

VERBS = {
    "quit": 0,
    "goodbye": 1,
    "break": 2,
    "reset": 3,
    "set": 4,
    "reassign": 5,
    "if": 6,
    "endif": 7,
    "exec": 8,
    "back": 9,
    "front": 10,
    "img": 11,
    "activate": 12,
    "deactivate": 13,
    "killframe": 14,
    "makeframe": 15,
    "focus": 16,
    "scrollbound": 17,
    "view": 18,
    "move": 19,
    "place": 20,
    "take": 21,
    "takeall": 22,
    "rebrand": 23,
    "remove": 24,
    "add": 25,
    "hitboxes": 26,
    "hurtboxes": 27,
    "create": 28,
    "update": 29,
    "sfx": 30,
    "song": 31,
    "sfxoff": 32,
    "songoff": 33,
    "offsetbgscrollx": 34,
    "offsetbgscrolly": 35,
    "for": 36,
    "endfor": 37,
    "print": 38,
    "update_sticks": 39,
}
VERBCOUNT = {}
for key in VERBS.keys():
    VERBCOUNT[key] = 0

script_data_dot_c = """// Generated with python from the red pants engine
#include "scripts.h"

void scripts_load() {
"""

scriptKey = 0
for mapKey in SCRIPTS.keys():
    script_data_dot_c += f"\n"
    script_data_dot_c += f"    add_script_map(\"{mapKey}\");\n"
    for key in SCRIPTS[mapKey]:
        state, frame = key.split(":") if ":" in key else (key, "0")
        script_data_dot_c += f"\n    add_script({scriptKey});\n"
        script_data_dot_c += f"    add_script_to_script_map(\"{mapKey}\", \"{state}\", {frame}, {scriptKey});\n"
        for i, statement in enumerate(SCRIPTS[mapKey][key]):
            if not statement: continue
            verb = statement[0]
            if verb.startswith("#"): continue
            if verb not in VERBS:
                print(f"The lost verb: {verb}")
                print(mapKey, key, statement)
                continue
            script_data_dot_c += f"""
    //{verb} {statement}
    Statement* s{scriptKey}_{i} = new_statement({VERBS[verb]});
    add_statement_to_script({scriptKey}, s{scriptKey}_{i});\n"""
            VERBCOUNT[verb] += 1
            for j, token in enumerate(statement[1:]):
                if token in OPERATORS:
                    script_data_dot_c += f"""
    SyntaxNode* sn{scriptKey}_{i}_{j} = new_syntax_node(OPERATOR);
    sn{scriptKey}_{i}_{j}->data.i = {OPERATORS[token]};\n"""
                elif token in LITERAL_KEYWORDS:
                    script_data_dot_c += f"""
    SyntaxNode* sn{scriptKey}_{i}_{j} = new_syntax_node({LITERAL_KEYWORDS[token]});\n"""
                else:
                    try:
                        int(token)
                        script_data_dot_c += f"""
    SyntaxNode* sn{scriptKey}_{i}_{j} = new_syntax_node(INT);
    sn{scriptKey}_{i}_{j}->data.i = {token};\n"""
                    except ValueError:
                        try:
                            float(token)
                            script_data_dot_c += f"""
    SyntaxNode* sn{scriptKey}_{i}_{j} = new_syntax_node(FLOAT);
    sn{scriptKey}_{i}_{j}->data.f = {token};\n"""
                        except ValueError:
                            if "." in token:
                                script_data_dot_c += f"""
    SyntaxNode* sn{scriptKey}_{i}_{j}_;\n"""
                                words = token.split(".");
                                first = words.pop(0);
                                script_data_dot_c += f"""
    sn{scriptKey}_{i}_{j}_ = new_syntax_node(STRING);
    sn{scriptKey}_{i}_{j}_->data.s = (char*)malloc({len(first) + 1});
    strncpy(sn{scriptKey}_{i}_{j}_->data.s, "{first}", {len(first) + 1});
    add_node_to_statement(s{scriptKey}_{i}, sn{scriptKey}_{i}_{j}_);\n"""
                                for word in words:
                                    script_data_dot_c += f"""
    sn{scriptKey}_{i}_{j}_ = new_syntax_node(DOT);
    add_node_to_statement(s{scriptKey}_{i}, sn{scriptKey}_{i}_{j}_);               
    sn{scriptKey}_{i}_{j}_ = new_syntax_node(STRING);
    sn{scriptKey}_{i}_{j}_->data.s = (char*)malloc({len(word) + 1});
    strncpy(sn{scriptKey}_{i}_{j}_->data.s, "{word}", {len(word) + 1});
    add_node_to_statement(s{scriptKey}_{i}, sn{scriptKey}_{i}_{j}_);\n"""
                                continue
                            
                            script_data_dot_c += f"""
    SyntaxNode* sn{scriptKey}_{i}_{j} = new_syntax_node(STRING);
    sn{scriptKey}_{i}_{j}->data.s = (char*)malloc({len(token) + 1});
    strncpy(sn{scriptKey}_{i}_{j}->data.s, "{token}", {len(token) + 1});\n"""

                script_data_dot_c += f"    add_node_to_statement(s{scriptKey}_{i}, sn{scriptKey}_{i}_{j});\n"
        scriptKey += 1
script_data_dot_c += "}\n"

with open("scriptdata.c", "w+") as f:
    f.write(script_data_dot_c)

from pprint import pprint
pprint(VERBCOUNT)

