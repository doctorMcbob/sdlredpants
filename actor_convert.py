import ACTORS
import actor

actordata_dot_c = """
// The following code was generated by the Red Pants Engine
// lets go baby

# include "actors.h"
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>

void actor_load() {
"""

for name in ACTORS.ACTORS.keys():
    template = ACTORS.ACTORS[name]
    a = actor.Actor(template)
    actordata_dot_c += f"    add_actor(\"{template['name']}\", {a.x}, {a.y}, {a.w}, {a.h}, {a.x_vel}, {a.y_vel}, NULL, NULL, \"{template['scripts']}\", \"{template['sprites']}\", NULL, {a._input_name if a._input_name else 'NULL'}, \"{a.state}\", {a.frame}, {a.direction}, {a.rotation}, {int(a.platform)}, {int(a.tangible)}, {int(a.physics)}, {int(a.updated)});\n"
    
actordata_dot_c += "}\n"

with open("actordata.c", "w+") as f:
    f.write(actordata_dot_c)

