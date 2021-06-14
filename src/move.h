#ifndef _MOVE_H_
#define _MOVE_H_

#include "position.h"

#include <stdint.h>

// The maximum number of pseudo legal moves per position.
#define MAX_NUM_PSEUDO_LEGAL_MOVES 256

typedef struct Move {
	uint8_t src, dst, promotion_piece;
} Move;

typedef struct UnmakeInfo {
	uint8_t captured_piece;
	bool promotion;
	uint8_t prev_castling_ability;
	uint8_t prev_en_passant_target_square;
	uint8_t prev_halfmove_clock;
	uint8_t castling_rook_src_square;
	uint8_t castling_rook_dst_square;
} UnmakeInfo;

void make_move(Position *position, Move move);

void make_move_unmake(Position *position, Move move, UnmakeInfo *unmake_info);

void unmake_move(Position *position, Move move, UnmakeInfo unmake_info);

// Converts a move to a string using pure coordinate notation.
char *move_to_string(Move move);

// Converts a string with pure coordinate notation to a move.
// Returns NULL if the string is not in valid pure coordinate notation.
Move *move_from_string(const char *string);

#endif
