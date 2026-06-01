//
// Created by adman on 31.05.26.
//

#include <stdbool.h>

#ifndef CCHESS_TYPES_H
#define CCHESS_TYPES_H

#define PLAYER1_COLOR WHITE
#define PLAYER2_COLOR BLACK

#define BOARD_SIZE 8
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

#define TEXTURE_SIZE 150

#define TEXTURE_SOURCE (Rectangle){0, 0, TEXTURE_SIZE, TEXTURE_SIZE}


#define TILE_WIDTH (SCREEN_WIDTH / BOARD_SIZE)
#define TILE_HEIGHT  (SCREEN_HEIGHT / BOARD_SIZE)


#define TILE_WIDTH_F ((float)SCREEN_WIDTH / (float)BOARD_SIZE)
#define TILE_HEIGHT_F ((float)SCREEN_HEIGHT / (float)BOARD_SIZE)

#define VEC2_0 (Vector2){0, 0}

#define ENSURE_BOUNDS(var, min, max) \
do { \
if ((var) < (min)) (var) = (min); \
if ((var) > (max)) (var) = (max); \
} while (0)

typedef int piece_t;

enum {
    INVALID = -1,
    EMPTY,
    PLAYER1_PAWN,
    PLAYER1_ROOK,
    PLAYER1_BISHOP,
    PLAYER1_KNIGHT,
    PLAYER1_QUEEN,
    PLAYER1_KING,

    PLAYER2_PAWN,
    PLAYER2_ROOK,
    PLAYER2_BISHOP,
    PLAYER2_KNIGHT,
    PLAYER2_QUEEN,
    PLAYER2_KING,
};


typedef struct {
    int file, rank;
} pos_t;

typedef piece_t board_t[BOARD_SIZE * BOARD_SIZE];

typedef struct {
    int len;
    pos_t *data;
} pos_arr_t;


static int playerN(const piece_t piece) {
    if (piece == INVALID || piece == EMPTY) return -1;
    if (piece >= PLAYER2_PAWN) return 1;
    return 0;
}

static int player2mult(const int player) {
    if (player == 0) {
        return -1; // up
    }
    return 1; // down
}


static piece_t board_get(board_t board, const pos_t pos) {
    if (pos.file < 0 || pos.file >= BOARD_SIZE || pos.rank < 0 || pos.rank >= BOARD_SIZE) {
        return INVALID;
    }
    return board[pos.rank * BOARD_SIZE + pos.file];
}

static void board_set(board_t board, const pos_t pos, const piece_t piece) {
    board[pos.rank * BOARD_SIZE + pos.file] = piece;
}

static bool is_enemy(int self, piece_t piece) {
    int other_player = playerN(piece);
    if (other_player == -1) {
        return false;
    }
    return self != other_player;
}

static pos_t pos_add(pos_t pos, int file, int rank) {
    return (pos_t){.file = pos.file + file, .rank = pos.rank + rank};
}

#endif //CCHESS_TYPES_H
