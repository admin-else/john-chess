#include <raylib.h>
#include <stdlib.h>
#include <string.h>

#include "engine.h"
#include "types.h"
#include "moves.h"
#include "textures.h"


static const int max_shown_eval_advantage = 5;
static const int eval_bar_width = 25;


board_t default_board = {
    PLAYER2_ROOK, PLAYER2_KNIGHT, PLAYER2_BISHOP, PLAYER2_QUEEN, PLAYER2_KING, PLAYER2_BISHOP, PLAYER2_KNIGHT,
    PLAYER2_ROOK, PLAYER2_PAWN, PLAYER2_PAWN, PLAYER2_PAWN, PLAYER2_PAWN, PLAYER2_PAWN, PLAYER2_PAWN, PLAYER2_PAWN,
    PLAYER2_PAWN,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    PLAYER1_PAWN, PLAYER1_PAWN, PLAYER1_PAWN, PLAYER1_PAWN, PLAYER1_PAWN, PLAYER1_PAWN, PLAYER1_PAWN, PLAYER1_PAWN,
    PLAYER1_ROOK, PLAYER1_KNIGHT, PLAYER1_BISHOP, PLAYER1_QUEEN, PLAYER1_KING, PLAYER1_BISHOP, PLAYER1_KNIGHT,
    PLAYER1_ROOK
};


void draw_board(board_t board, int player) {
    // draw background
    const Color player1color = player ? PLAYER1_COLOR : PLAYER2_COLOR;
    const Color player2color = player ? PLAYER2_COLOR : PLAYER1_COLOR;

    ClearBackground(player1color);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i % 2 == 0 ^ j % 2 == 0) {
                DrawRectangle(i * TILE_WIDTH, j * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, player2color);
            }
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            const piece_t piece = board_get(board, (pos_t){.rank = i, .file = j});
            if (piece == EMPTY) {
                continue;
            }
            const Texture2D texture = piece_to_texture(piece);
            DrawTexturePro(texture, TEXTURE_SOURCE, (Rectangle){
                               (float) j * TILE_WIDTH_F, (float) i * TILE_HEIGHT_F, TILE_WIDTH_F, TILE_HEIGHT_F
                           }, VEC2_0, 0, WHITE);
        }
    }
}

pos_t pos_from_mouse(const Vector2 mouse_pos) {
    int board_x = (int) (mouse_pos.x / TILE_WIDTH_F);
    int board_y = (int) (mouse_pos.y / TILE_HEIGHT_F);
    ENSURE_BOUNDS(board_x, 0, BOARD_SIZE - 1);
    ENSURE_BOUNDS(board_y, 0, BOARD_SIZE - 1);
    return (pos_t){.rank = board_y, .file = board_x};
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "John Chess is calling");
    SetTargetFPS(60);

    load_piece_textures();

    board_t board;
    memcpy(board, default_board, sizeof(board_t));

    piece_t holding = EMPTY;
    pos_arr_t possible_moves = (pos_arr_t){.len = 0};
    pos_t initial_piece_pos = {};
    int turn = 0;
    float eval_value = 0;

    while (!WindowShouldClose()) {
        Vector2 mouse_pos = GetMousePosition();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (holding == EMPTY) {
                // take
                initial_piece_pos = pos_from_mouse(mouse_pos);
                holding = board_get(board, initial_piece_pos);
                if (turn == playerN(holding)) {
                    possible_moves = valid_moves(board, initial_piece_pos);
                    board_set(board, initial_piece_pos, EMPTY);
                }
            }
        } else if (holding != EMPTY) {
            // DROP
            pos_t new_pow = pos_from_mouse(mouse_pos);
            bool valid_move = false;
            for (int i = 0; i < possible_moves.len; i++) {
                if (possible_moves.data[i].file == new_pow.file && possible_moves.data[i].rank == new_pow.rank) {
                    valid_move = true;
                    break;
                }
            }
            if (!valid_move) {
                new_pow = initial_piece_pos;
            }
            if (holding == PLAYER1_PAWN && new_pow.rank == 0) {
                holding = PLAYER1_QUEEN;
            }
            if (holding == PLAYER2_PAWN && new_pow.rank == 7) {
                holding = PLAYER2_QUEEN;
            }

            board_set(board, new_pow, holding);
            holding = EMPTY;

            if (valid_move) {
                free(possible_moves.data);
                possible_moves = (pos_arr_t){.len = 0};

                // this decides if the bot or another player plays

                //turn ^= 1;
                bot_move(board, 1, 4);

                eval_value = symmetric_eval(board);
            }
        }

        BeginDrawing();
        draw_board(board, true);
        if (holding != EMPTY) {
            const Texture2D texture = piece_to_texture(holding);
            DrawTexturePro(texture, TEXTURE_SOURCE, (Rectangle){
                               mouse_pos.x - (float) TEXTURE_SIZE / 2, mouse_pos.y - (float) TEXTURE_SIZE / 2,
                               TILE_WIDTH_F, TILE_HEIGHT_F
                           }, VEC2_0, 0, WHITE);
        }

        for (int i = 0; i < possible_moves.len; i++) {
            DrawRectangle(TILE_WIDTH * possible_moves.data[i].file, TILE_HEIGHT * possible_moves.data[i].rank,
                          TILE_WIDTH, TILE_HEIGHT, (Color){255, 0, 0, 100});
        }

        DrawRectangle(0, 0, eval_bar_width, SCREEN_HEIGHT, WHITE);

        float size = (max_shown_eval_advantage - eval_value) * (SCREEN_HEIGHT / (max_shown_eval_advantage * 2));
        DrawRectangle(0, 0, eval_bar_width, size, BLACK);


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
