#ifndef _POSITION_H_
#define _POSITION_H_

#include <stdint.h>
#include <stdbool.h>

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

#define SQUARE_TO_FILE(square) (((square) % 10) - 1)
#define SQUARE_TO_RANK(square) (((square) / 10) - 2)

#define CASTLE_WHITE_KING 1
#define CASTLE_WHITE_QUEEN 2
#define CASTLE_BLACK_KING 4
#define CASTLE_BLACK_QUEEN 8
#define CASTLE_WHITE (CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN)
#define CASTLE_BLACK (CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN)
#define CASTLE_ALL (CASTLE_WHITE | CASTLE_BLACK)

typedef struct Position {
	uint8_t squares[120];
	uint8_t side_to_move;
	uint8_t castling_ability;
	uint8_t en_passant_target_square;
	uint8_t halfmove_clock;
	uint16_t fullmove_counter;
	uint64_t hash;
	int eval;
} Position;

extern const int8_t PAWN_CAPTURES[2];
extern const int8_t KNIGHT_MOVES[8];
extern const int8_t BISHOP_DIRECTIONS[4];
extern const int8_t ROOK_DIRECTIONS[4];
extern const int8_t QUEEN_DIRECTIONS[8];
extern const int8_t KING_MOVES[8];

// Initializes a position.
void position_init(Position *position);

// Prints the board of a position.
void position_print_board(const Position *position);

bool is_square_threatened(const Position *position, uint8_t threatened_side,
			  uint8_t threatened_square);

bool is_king_in_check(const Position *position, uint8_t side);

#endif
