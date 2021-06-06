#ifndef _MOVE_H_
#define _MOVE_H_

#include "position.h"

#include <stdint.h>

// The maximum number of moves per position.
// https://www.chessprogramming.org/Chess_Position
#define MAX_NUM_MOVES 218

typedef struct Move {
	uint8_t src, dst, promotion_piece;
} Move;

void make_move(Position *position, Move move);

// Converts a move to a string using pure coordinate notation.
char *move_to_string(Move move);

// Converts a string with pure coordinate notation to a move.
// Returns NULL if the string is not in valid pure coordinate notation.
Move *move_from_string(const char *string);

#endif
