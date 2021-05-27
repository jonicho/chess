#ifndef _MOVE_H_
#define _MOVE_H_

#include "board.h"

#include <stdint.h>

// The maximum number of moves per position.
// https://www.chessprogramming.org/Chess_Position
#define MAX_NUM_MOVES 218

typedef struct Move {
	uint8_t src, dst;
} Move;

void make_move(Board *board, Move *move);

// Converts a move to a string using pure coordinate notation.
char *move_to_string(Move *move);
#endif
