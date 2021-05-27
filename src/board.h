#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdint.h>

#define WHITE 0
#define BLACK 8

#define EMPTY 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

#define WHITE_PAWN PAWN | WHITE
#define WHITE_KNIGHT KNIGHT | WHITE
#define WHITE_BISHOP BISHOP | WHITE
#define WHITE_ROOK ROOK | WHITE
#define WHITE_QUEEN QUEEN | WHITE
#define WHITE_KING KING | WHITE

#define BLACK_PAWN PAWN | BLACK
#define BLACK_KNIGHT KNIGHT | BLACK
#define BLACK_BISHOP BISHOP | BLACK
#define BLACK_ROOK ROOK | BLACK
#define BLACK_QUEEN QUEEN | BLACK
#define BLACK_KING KING | BLACK

#define PIECE_TYPE(x) ((x)&7)
#define PIECE_COLOR(x) ((x)&8)

#define OFF_BOARD 0xff

// Converts rank and file to square index.
#define RF(rank, file) (((rank) + 2) * 10 + ((file) + 1))

#define CASTLE_WHITE_KING 1
#define CASTLE_WHITE_QUEEN 2
#define CASTLE_BLACK_KING 4
#define CASTLE_BLACK_QUEEN 8
#define CASTLE_WHITE CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN
#define CASTLE_BLACK CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN
#define CASTLE_ALL CASTLE_WHITE | CASTLE_BLACK

typedef struct Board {
	uint8_t squares[120];
	uint8_t side_to_move;
	uint8_t castling_ability;
	uint8_t en_passant_target_square;
	uint8_t halfmove_clock;
	uint16_t fullmove_counter;
} Board;

// Initializes a board.
void board_init(Board *board);

// Prints a board.
void board_print(Board *board);

// Prints a board including the off-board-squares for debugging.
void board_print_debug(Board *board);

#endif
