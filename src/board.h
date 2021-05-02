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

typedef struct Board {
	uint8_t squares[64];
	uint8_t side_to_move;
} Board;

void board_print(Board *board);

#endif
