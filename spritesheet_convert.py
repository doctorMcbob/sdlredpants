import SPRITESHEETS

spritesheets_dot_c = """
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>

void load_spritesheet(SDL_Renderer* rend,
		      const char* filename,
		      const char* names[],
		      int xs[],
		      int ys[],
		      int ws[],
		      int hs[],
		      int count);

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

spritesheets_dot_c += "}\n"

with open("spritesheets.c", "w+") as f:
    f.write(spritesheets_dot_c)

