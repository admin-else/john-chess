#include <raylib.h>
#include <stddef.h>

#include "../types.h"
//
// Created by adman on 31.05.26.
//
typedef struct {
    Texture2D white_pawn;
    Texture2D white_rook;
    Texture2D white_bishop;
    Texture2D white_knight;
    Texture2D white_queen;
    Texture2D white_king;

    Texture2D black_pawn;
    Texture2D black_rook;
    Texture2D black_bishop;
    Texture2D black_knight;
    Texture2D black_queen;
    Texture2D black_king;
} piece_textures_t;

piece_textures_t piece_textures;

void load_piece_textures() {
    piece_textures.white_pawn = LoadTexture("../assets/pieces/white/pawn.png");
    piece_textures.white_rook = LoadTexture("../assets/pieces/white/rook.png");
    piece_textures.white_bishop = LoadTexture("../assets/pieces/white/bishop.png");
    piece_textures.white_knight = LoadTexture("../assets/pieces/white/knight.png");
    piece_textures.white_queen = LoadTexture("../assets/pieces/white/queen.png");
    piece_textures.white_king = LoadTexture("../assets/pieces/white/king.png");

    piece_textures.black_pawn = LoadTexture("../assets/pieces/black/pawn.png");
    piece_textures.black_rook = LoadTexture("../assets/pieces/black/rook.png");
    piece_textures.black_bishop = LoadTexture("../assets/pieces/black/bishop.png");
    piece_textures.black_knight = LoadTexture("../assets/pieces/black/knight.png");
    piece_textures.black_queen = LoadTexture("../assets/pieces/black/queen.png");
    piece_textures.black_king = LoadTexture("../assets/pieces/black/king.png");
}

// piece_t MUST NOT BE EMPTY CHECK THAT BEFRORE
Texture2D piece_to_texture(piece_t piece) {
    switch (piece) {
        case PLAYER1_PAWN:
            return piece_textures.white_pawn;
        case PLAYER1_ROOK:
            return piece_textures.white_rook;
        case PLAYER1_BISHOP:
            return piece_textures.white_bishop;
        case PLAYER1_KNIGHT:
            return piece_textures.white_knight;
        case PLAYER1_QUEEN:
            return piece_textures.white_queen;
        case PLAYER1_KING:
            return piece_textures.white_king;
        case PLAYER2_PAWN:
            return piece_textures.black_pawn;
        case PLAYER2_ROOK:
            return piece_textures.black_rook;
        case PLAYER2_BISHOP:
            return piece_textures.black_bishop;
        case PLAYER2_KNIGHT:
            return piece_textures.black_knight;
        case PLAYER2_QUEEN:
            return piece_textures.black_queen;
        case PLAYER2_KING:
            return piece_textures.black_king;
        default:
            break;
            //unreachable();
    }
}
