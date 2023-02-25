import BOXES

boxdata_dot_c = """# include "boxes.h"
# include <SDL2/SDL.h>

void boxes_load() {
"""

for mapkey in BOXES.HITBOXES.keys():
    boxdata_dot_c += f"    add_hitbox_map(\"{mapkey}\");\n"
    for key in BOXES.HITBOXES[mapkey]:
        if not BOXES.HITBOXES[mapkey][key]: continue
        count = len(BOXES.HITBOXES[mapkey][key])
        xs = [rect[0][0] for rect in BOXES.HITBOXES[mapkey][key]]
        ys = [rect[0][1] for rect in BOXES.HITBOXES[mapkey][key]]
        ws = [rect[1][0] for rect in BOXES.HITBOXES[mapkey][key]]
        hs = [rect[1][1] for rect in BOXES.HITBOXES[mapkey][key]]
        state, frame = key.split(":") if ":" in key else (key, "0")
        boxdata_dot_c += f"    int i{mapkey}{state}{frame}xs[{count}] = " + "{" + ", ".join(str(v) for v  in xs) + "};\n"
        boxdata_dot_c += f"    int i{mapkey}{state}{frame}ys[{count}] = " + "{" + ", ".join(str(v) for v  in ys) + "};\n"
        boxdata_dot_c += f"    int i{mapkey}{state}{frame}ws[{count}] = " + "{" + ", ".join(str(v) for v  in ws) + "};\n"
        boxdata_dot_c += f"    int i{mapkey}{state}{frame}hs[{count}] = " + "{" + ", ".join(str(v) for v  in hs) + "};\n"
        boxdata_dot_c += f"    add_to_hitbox_map(\"{mapkey}\", \"{state}\", {frame}, i{mapkey}{state}{frame}xs, i{mapkey}{state}{frame}ys, i{mapkey}{state}{frame}hs, i{mapkey}{state}{frame}ws, {count});\n"


for mapkey in BOXES.HURTBOXES.keys():
    boxdata_dot_c += f"    add_hurtbox_map(\"{mapkey}\");\n"
    for key in BOXES.HURTBOXES[mapkey]:
        if not BOXES.HURTBOXES[mapkey][key]: continue
        count = len(BOXES.HURTBOXES[mapkey][key])
        xs = [rect[0][0] for rect in BOXES.HURTBOXES[mapkey][key]]
        ys = [rect[0][1] for rect in BOXES.HURTBOXES[mapkey][key]]
        ws = [rect[1][0] for rect in BOXES.HURTBOXES[mapkey][key]]
        hs = [rect[1][1] for rect in BOXES.HURTBOXES[mapkey][key]]
        state, frame = key.split(":") if ":" in key else (key, "0")
        boxdata_dot_c += f"    int u{mapkey}{state}{frame}xs[{count}] = " + "{" + ", ".join(str(v) for v  in xs) + "};\n"
        boxdata_dot_c += f"    int u{mapkey}{state}{frame}ys[{count}] = " + "{" + ", ".join(str(v) for v  in ys) + "};\n"
        boxdata_dot_c += f"    int u{mapkey}{state}{frame}ws[{count}] = " + "{" + ", ".join(str(v) for v  in ws) + "};\n"
        boxdata_dot_c += f"    int u{mapkey}{state}{frame}hs[{count}] = " + "{" + ", ".join(str(v) for v  in hs) + "};\n"
        boxdata_dot_c += f"    add_to_hurtbox_map(\"{mapkey}\", \"{state}\", {frame}, u{mapkey}{state}{frame}xs, u{mapkey}{state}{frame}ys, u{mapkey}{state}{frame}hs, u{mapkey}{state}{frame}ws, {count});\n"


boxdata_dot_c += "}\n"

with open("boxdata.c", "w+") as f:
    f.write(boxdata_dot_c)

