//
// Created by adman on 31.05.26.
//

#ifndef CCHESS_MOVES_H
#define CCHESS_MOVES_H
#include "types.h"

// soon other state like have we castled etc should be part of board probably
pos_arr_t valid_moves(board_t board, pos_t from);

#endif //CCHESS_MOVES_H
