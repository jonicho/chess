#ifndef _MOVE_H_
#define _MOVE_H_

#include "board.h"

#include <stdint.h>

typedef struct Move {
	uint8_t src, dst;
} Move;

void make_move(Board *board, Move *move);

#endif
