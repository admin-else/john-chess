//
// Created by adman on 31.05.26.
//


#include <stddef.h>

#include "../types.h"

static const int DIRECTIONS_ROOK[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
static const int DIRECTIONS_BISHOP[][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
static const int DIRECTIONS_QUEEN[][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}};
static const int DIRECTIONS_KING[][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
static const int DIRECTIONS_KNIGHT[][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

int nmoves_pawn(board_t board, pos_t pos, int player) {
    int n = 0;
    if (board_get(board, pos_add(pos, 0, player2mult(player))) == EMPTY) {
        n++;
        if ((player == 0 && pos.rank == 6) || (player == 1 && pos.rank == 1)) {
            if (board_get(board, pos_add(pos, 0, player2mult(player) * 2)) == EMPTY) {
                n++;
            }
        }
    }
    if (is_enemy(player, board_get(board, pos_add(pos, -1, player2mult(player))))) {
        n++;
    }
    if (is_enemy(player, board_get(board, pos_add(pos, 1, player2mult(player))))) {
        n++;
    }
    return n;
}

int nmoves_rook(board_t board, pos_t pos, int player) {
    int n = 0;
    for (int i = 0; i < 4; i++) {
        const int direction[2] = {[0] = DIRECTIONS_ROOK[i][0], [1] = DIRECTIONS_ROOK[i][1]};
        for (int j = 1; j < 8; j++) {
            const piece_t piece = board_get(board, pos_add(pos, direction[0] * j, direction[1] * j));
            if (piece == EMPTY || is_enemy(player, piece)) {
                n++;
            }
            if (piece != EMPTY) {
                break;
            }
        }
    }
    return n;
}

int nmoves_queen(board_t board, pos_t pos, int player) {
    int n = 0;
    for (int i = 0; i < 8; i++) {
        const int direction[2] = {[0] = DIRECTIONS_QUEEN[i][0], [1] = DIRECTIONS_QUEEN[i][1]};
        for (int j = 1; j < 8; j++) {
            const piece_t piece = board_get(board, pos_add(pos, direction[0] * j, direction[1] * j));
            if (piece == EMPTY || is_enemy(player, piece)) {
                n++;
            }
            if (piece != EMPTY) {
                break;
            }
        }
    }
    return n;
}


int nmoves_bishop(board_t board, pos_t pos, int player) {
    int n = 0;
    for (int i = 0; i < 4; i++) {
        const int direction[2] = {[0] = DIRECTIONS_BISHOP[i][0], [1] = DIRECTIONS_BISHOP[i][1]};
        for (int j = 1; j < 8; j++) {
            const piece_t piece = board_get(board, pos_add(pos, direction[0] * j, direction[1] * j));
            if (piece == EMPTY || is_enemy(player, piece)) {
                n++;
            }
            if (piece != EMPTY) {
                break;
            }
        }
    }
    return n;
}

int nmoves_king(board_t board, pos_t pos, int player) {
    int n = 0;
    for (int i = 0; i < 8; i++) {
        const int direction[2] = {[0] = DIRECTIONS_KING[i][0], [1] = DIRECTIONS_KING[i][1]};
        const piece_t piece = board_get(board, pos_add(pos, direction[0], direction[1]));
        if (piece == EMPTY || is_enemy(player, piece)) {
            n++;
        }
    }
    return n;
}

int nmoves_knight(board_t board, pos_t pos, int player) {
    int n = 0;
    for (int i = 0; i < 8; i++) {
        const int direction[2] = {[0] = DIRECTIONS_KNIGHT[i][0], [1] = DIRECTIONS_KNIGHT[i][1]};
        const piece_t piece = board_get(board, pos_add(pos, direction[0], direction[1]));
        if (piece == EMPTY || is_enemy(player, piece)) {
            n++;
        }
    }
    return n;
}

int nvalid_moves(board_t board, pos_t pos) {
    const piece_t moving = board_get(board, pos);
    int player = playerN(moving);

    switch (moving) {
        case EMPTY:
            return 0;
        case PLAYER1_PAWN:
        case PLAYER2_PAWN:
            return nmoves_pawn(board, pos, player);
        case PLAYER1_ROOK:
        case PLAYER2_ROOK:
            return nmoves_rook(board, pos, player);
        case PLAYER1_BISHOP:
        case PLAYER2_BISHOP:
            return nmoves_bishop(board, pos, player);
        case PLAYER1_QUEEN:
        case PLAYER2_QUEEN:
            return nmoves_queen(board, pos, player);
        case PLAYER1_KING:
        case PLAYER2_KING:
            return nmoves_king(board, pos, player);
        case PLAYER1_KNIGHT:
        case PLAYER2_KNIGHT:
            return nmoves_knight(board, pos, player);
        default:
            break;
            //unreachable();
    }
}

int nvalid_moves_board(board_t board, int player) {
    int n = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (playerN(board_get(board, (pos_t){.rank = i, .file = j})) == player) {
                n += nvalid_moves(board, (pos_t){.rank = i, .file = j});
            }
        }
    }
    return n;
}
