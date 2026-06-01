#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "moves.h"
#include "types.h"
#include "nmoves.h"

static pos_t find_king(board_t board, int player) {
    piece_t king = player == 0 ? PLAYER1_KING : PLAYER2_KING;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board_get(board, (pos_t){i, j}) == king) {
                return (pos_t){i, j};
            }
        }
    }
    return (pos_t){-1, -1};
}

static bool is_square_attacked(board_t board, pos_t pos, int by_player) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pos_t piece_pos = (pos_t){i, j};
            if (playerN(board_get(board, piece_pos)) != by_player) {
                continue;
            }
            pos_arr_t attacks = valid_moves(board, piece_pos);
            for (int k = 0; k < attacks.len; k++) {
                if (attacks.data[k].file == pos.file && attacks.data[k].rank == pos.rank) {
                    free(attacks.data);
                    return true;
                }
            }
            free(attacks.data);
        }
    }
    return false;
}

static bool is_player_in_check(board_t board, int player) {
    pos_t king_pos = find_king(board, player);
    if (king_pos.file < 0) return false;
    int enemy = player ^ 1;
    return is_square_attacked(board, king_pos, enemy);
}

// https://www.chessprogramming.org/Evaluation#Where_to_Start
float symmetric_eval(board_t b) {
    float score = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            switch (board_get(b, (pos_t){i, j})) {
                case PLAYER1_KING:
                    score += 200;
                    break;
                case PLAYER2_KING:
                    score -= 200;
                    break;
                case PLAYER1_QUEEN:
                    score += 9;
                    break;
                case PLAYER2_QUEEN:
                    score -= 9;
                    break;
                case PLAYER1_ROOK:
                    score += 5;
                    break;
                case PLAYER2_ROOK:
                    score -= 5;
                    break;
                case PLAYER1_BISHOP:
                case PLAYER1_KNIGHT:
                    score += 3;
                    break;
                case PLAYER2_BISHOP:
                case PLAYER2_KNIGHT:
                    score -= 3;
                    break;
                case PLAYER1_PAWN:
                    score += 1;
                    break;
                case PLAYER2_PAWN:
                    score -= 1;
                    break;
                default:
                    break;
            }
        }
    }
    // Mobility bonus
    score += (float) nvalid_moves_board(b, 0) * (float) 0.1;
    score -= (float) nvalid_moves_board(b, 1) * (float) 0.1;

    return score;
}

int debugCallsNM = 0;

float negated_minimax(board_t board, int depth, int player, float alpha, float beta) {
    debugCallsNM++;
    if (depth == 0) {
        float eval = symmetric_eval(board);
        return player == 0 ? eval : -eval;
    }
    float max = -INFINITY;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pos_t pos = (pos_t){i, j};
            if (playerN(board_get(board, pos)) != player) {
                continue;
            }
            pos_arr_t moves = valid_moves(board, pos);
            if (moves.len == 0) {
                continue;
            }
            for (int k = 0; k < moves.len; k++) {
                board_t next_board;
                memcpy(next_board, board, sizeof(board_t));
                piece_t holding = board_get(next_board, pos);
                board_set(next_board, pos, EMPTY);
                board_set(next_board, moves.data[k], holding);
                if (is_player_in_check(board, player)) {
                    continue;
                }

                float score = -negated_minimax(next_board, depth - 1, player ^ 1, -beta, -alpha);
                if (score > max) {
                    max = score;
                    if (score > alpha) {
                        alpha = score;
                    }
                }
                if (score >= beta) {
                    free(moves.data);
                    return score;
                }
            }
            free(moves.data);
        }
    }
    return max;
}

void bot_move(board_t board, int player, int depth) {
    float max = -INFINITY;
    pos_t from = (pos_t){0, 0};
    pos_t to = (pos_t){0, 0};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pos_t pos = (pos_t){i, j};
            piece_t piece = board_get(board, pos);
            if (playerN(piece) != player) {
                continue;
            }
            pos_arr_t moves = valid_moves(board, pos);
            if (moves.len == 0) {
                continue;
            }
            for (int k = 0; k < moves.len; k++) {
                board_t next_board;
                memcpy(next_board, board, sizeof(board_t));
                piece_t holding = board_get(next_board, pos);
                board_set(next_board, pos, EMPTY);
                board_set(next_board, moves.data[k], holding);
                if (is_player_in_check(board, player)) {
                    continue;
                }
                float score = -negated_minimax(next_board, depth, player ^ 1, -INFINITY, INFINITY);
                if (score > max) {
                    from = pos;
                    to = moves.data[k];
                    max = score;
                }
            }
            free(moves.data);
        }
    }
    piece_t holding = board_get(board, from);
    board_set(board, from, EMPTY);
    board_set(board, to, holding);
    printf("Bot move: %d %d %d %d called minimax %d times\n", from.file, from.rank, to.file, to.rank, debugCallsNM);
}
