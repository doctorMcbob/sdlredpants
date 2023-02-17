import SPRITESHEETS

spritesheets_dot_c = """# include "sprites.h"
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>

void spritesheet_load(SDL_Renderer* rend) {
"""

for filename in SPRITESHEETS.SPRITESHEETS.keys():
    names = list(SPRITESHEETS.SPRITESHEETS[filename].keys())
    xs    = [SPRITESHEETS.SPRITESHEETS[filename][name][0][0] for name in names]
    ys    = [SPRITESHEETS.SPRITESHEETS[filename][name][0][1] for name in names]
    ws    = [SPRITESHEETS.SPRITESHEETS[filename][name][1][0] for name in names]
    hs    = [SPRITESHEETS.SPRITESHEETS[filename][name][1][1] for name in names]
    length = len(names)
    name = filename.split(".")[0]
    
    spritesheets_dot_c += f"    const char* {name}names[{length}] = " + "{" + repr(names)[1:-1].replace("'", '"') + "};\n"
    spritesheets_dot_c += f"    int {name}xs[{length}] = " + "{" + ", ".join(str(v) for v in xs) + "};\n"
    spritesheets_dot_c += f"    int {name}ys[{length}] = " + "{" + ", ".join(str(v) for v in ys) + "};\n"
    spritesheets_dot_c += f"    int {name}ws[{length}] = " + "{" + ", ".join(str(v) for v in ws) + "};\n"
    spritesheets_dot_c += f"    int {name}hs[{length}] = " + "{" + ", ".join(str(v) for v in hs) + "};\n"


    spritesheets_dot_c += f"""
    load_spritesheet(rend, "img/{filename}", {name}names, {name}xs, {name}ys, {name}ws, {name}hs, {length});
"""

keys = set()
for key in SPRITESHEETS.OFFSETS.keys():
    if type(SPRITESHEETS.OFFSETS[key]) == dict:
        for name in SPRITESHEETS.OFFSETS[key].keys():
            if name in keys: continue
            x, y = SPRITESHEETS.OFFSETS[key][name]
            spritesheets_dot_c += f"    add_offset(\"{name}\", {x}, {y});\n"
            keys.add(name)
    else:
        if key in keys: continue
        x, y = SPRITESHEETS.OFFSETS[key]
        spritesheets_dot_c += f"    add_offset(\"{key}\", {x}, {y});\n"
        keys.add(key)
    
spritesheets_dot_c += "}\n"

with open("spritesheets.c", "w+") as f:
    f.write(spritesheets_dot_c)

