//
// Created by adman on 31.05.26.
//

#ifndef JOHN_CHESS_ENGINE_H
#define JOHN_CHESS_ENGINE_H
#include "types.h"

float symmetric_eval(board_t b);

void bot_move(board_t board, int player, int depth);

#endif //JOHN_CHESS_ENGINE_H
