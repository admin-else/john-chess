//
// Created by adman on 31.05.26.
//


#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

static const int DIRECTIONS_ROOK[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
static const int DIRECTIONS_BISHOP[][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
static const int DIRECTIONS_QUEEN[][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}};
static const int DIRECTIONS_KING[][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
static const int DIRECTIONS_KNIGHT[][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

pos_arr_t moves_pawn(board_t board, pos_t pos, int player) {
    pos_t possible_moves[4];
    int n = 0;
    pos_t forward1 = (pos_t){.rank = pos.rank + player2mult(player) * 1, .file = pos.file};
    if (board_get(board, forward1) == EMPTY) {
        possible_moves[n++] = forward1;
        if ((player == 0 && pos.rank == 6) || (player == 1 && pos.rank == 1)) {
            pos_t forward2 = (pos_t){.rank = pos.rank + player2mult(player) * 2, .file = pos.file};
            if (board_get(board, forward2) == EMPTY) {
                possible_moves[n++] = forward2;
            }
        }
    }
    pos_t forward_diag1 = (pos_t){.rank = pos.rank + player2mult(player) * 1, .file = pos.file - 1};
    if (is_enemy(player, board_get(board, forward_diag1))) {
        possible_moves[n++] = forward_diag1;
    }

    pos_t forward_diag2 = (pos_t){.rank = pos.rank + player2mult(player) * 1, .file = pos.file + 1};
    if (is_enemy(player, board_get(board, forward_diag2))) {
        possible_moves[n++] = forward_diag2;
    }
    if (n == 0) {
        return (pos_arr_t){.len = 0};
    }

    pos_t *possible_moves_dyn = malloc(sizeof(pos_t) * n);
    memcpy(possible_moves_dyn, possible_moves, sizeof(pos_t) * n);
    return (pos_arr_t){.len = n, .data = possible_moves_dyn};
}

pos_arr_t moves_rook(board_t board, pos_t pos, int player) {
    pos_t possible_moves[14];
    int n = 0;
    for (int i = 0; i < 4; i++) {
        const int direction[2] = {[0] = DIRECTIONS_ROOK[i][0], [1] = DIRECTIONS_ROOK[i][1]};
        for (int j = 1; j < 8; j++) {
            const piece_t piece = board_get(board, pos_add(pos, direction[0] * j, direction[1] * j));
            if (piece == EMPTY || is_enemy(player, piece)) {
                possible_moves[n++] = pos_add(pos, direction[0] * j, direction[1] * j);
            }
            if (piece != EMPTY) {
                break;
            }
        }
    }
    if (n == 0) {
        return (pos_arr_t){.len = 0};
    }

    pos_t *possible_moves_dyn = malloc(sizeof(pos_t) * n);
    memcpy(possible_moves_dyn, possible_moves, sizeof(pos_t) * n);
    return (pos_arr_t){.len = n, .data = possible_moves_dyn};
}

pos_arr_t moves_queen(board_t board, pos_t pos, int player) {
    pos_t possible_moves[28];
    int n = 0;
    for (int i = 0; i < 8; i++) {
        const int direction[2] = {[0] = DIRECTIONS_QUEEN[i][0], [1] = DIRECTIONS_QUEEN[i][1]};
        for (int j = 1; j < 8; j++) {
            const piece_t piece = board_get(board, pos_add(pos, direction[0] * j, direction[1] * j));
            if (piece == EMPTY || is_enemy(player, piece)) {
                possible_moves[n++] = pos_add(pos, direction[0] * j, direction[1] * j);
            }
            if (piece != EMPTY) {
                break;
            }
        }
    }
    if (n == 0) {
        return (pos_arr_t){.len = 0};
    }

    pos_t *possible_moves_dyn = malloc(sizeof(pos_t) * n);
    memcpy(possible_moves_dyn, possible_moves, sizeof(pos_t) * n);
    return (pos_arr_t){.len = n, .data = possible_moves_dyn};
}


pos_arr_t moves_bishop(board_t board, pos_t pos, int player) {
    pos_t possible_moves[14];
    int n = 0;
    for (int i = 0; i < 4; i++) {
        const int direction[2] = {[0] = DIRECTIONS_BISHOP[i][0], [1] = DIRECTIONS_BISHOP[i][1]};
        for (int j = 1; j < 8; j++) {
            const piece_t piece = board_get(board, pos_add(pos, direction[0] * j, direction[1] * j));
            if (piece == EMPTY || is_enemy(player, piece)) {
                possible_moves[n++] = pos_add(pos, direction[0] * j, direction[1] * j);
            }
            if (piece != EMPTY) {
                break;
            }
        }
    }
    if (n == 0) {
        return (pos_arr_t){.len = 0};
    }

    pos_t *possible_moves_dyn = malloc(sizeof(pos_t) * n);
    memcpy(possible_moves_dyn, possible_moves, sizeof(pos_t) * n);
    return (pos_arr_t){.len = n, .data = possible_moves_dyn};
}

pos_arr_t moves_king(board_t board, pos_t pos, int player) {
    pos_t possible_moves[8];
    int n = 0;
    for (int i = 0; i < 8; i++) {
        const int direction[2] = {[0] = DIRECTIONS_KING[i][0], [1] = DIRECTIONS_KING[i][1]};
        const piece_t piece = board_get(board, pos_add(pos, direction[0], direction[1]));
        if (piece == EMPTY || is_enemy(player, piece)) {
            possible_moves[n++] = pos_add(pos, direction[0], direction[1]);
        }
    }
    if (n == 0) {
        return (pos_arr_t){.len = 0};
    }
    pos_t *possible_moves_dyn = malloc(sizeof(pos_t) * n);
    memcpy(possible_moves_dyn, possible_moves, sizeof(pos_t) * n);
    return (pos_arr_t){.len = n, .data = possible_moves_dyn};
}

pos_arr_t moves_knight(board_t board, pos_t pos, int player) {
    pos_t possible_moves[8];
    int n = 0;
    for (int i = 0; i < 8; i++) {
        const int direction[2] = {[0] = DIRECTIONS_KNIGHT[i][0], [1] = DIRECTIONS_KNIGHT[i][1]};
        const piece_t piece = board_get(board, pos_add(pos, direction[0], direction[1]));
        if (piece == EMPTY || is_enemy(player, piece)) {
            possible_moves[n++] = pos_add(pos, direction[0], direction[1]);
        }
    }
    if (n == 0) {
        return (pos_arr_t){.len = 0};
    }
    pos_t *possible_moves_dyn = malloc(sizeof(pos_t) * n);
    memcpy(possible_moves_dyn, possible_moves, sizeof(pos_t) * n);
    return (pos_arr_t){.len = n, .data = possible_moves_dyn};
}

pos_arr_t valid_moves(board_t board, pos_t pos) {
    const piece_t moving = board_get(board, pos);
    int player = playerN(moving);

    switch (moving) {
        case EMPTY:
            return (pos_arr_t){.len = 0};
        case PLAYER1_PAWN:
        case PLAYER2_PAWN:
            return moves_pawn(board, pos, player);
        case PLAYER1_ROOK:
        case PLAYER2_ROOK:
            return moves_rook(board, pos, player);
        case PLAYER1_BISHOP:
        case PLAYER2_BISHOP:
            return moves_bishop(board, pos, player);
        case PLAYER1_QUEEN:
        case PLAYER2_QUEEN:
            return moves_queen(board, pos, player);
        case PLAYER1_KING:
        case PLAYER2_KING:
            return moves_king(board, pos, player);
        case PLAYER1_KNIGHT:
        case PLAYER2_KNIGHT:
            return moves_knight(board, pos, player);
        default:
    }
    // unreachable
    return (pos_arr_t){.len = 0};
}
