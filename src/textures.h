//
// Created by adman on 31.05.26.
//

#ifndef JOHN_CHESS_TEXTURES_H
#define JOHN_CHESS_TEXTURES_H

#include <raylib.h>
#include "../types.h"

void load_piece_textures();

Texture2D piece_to_texture(piece_t piece);

#endif //JOHN_CHESS_TEXTURES_H
