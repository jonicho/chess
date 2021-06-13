#ifndef _GAME_H_
#define _GAME_H_

#include "move.h"
#include "position.h"

#include <stdbool.h>
#include <stddef.h>

#define INITIAL_MOVES_CAPACITY 128

typedef struct Game {
	Position *current_position;
	Move *moves;
	size_t moves_capacity;
	size_t num_moves;
} Game;

// Initializes a game.
void game_init(Game *game);

// Make a move in the game.
// Returns false if the move is not a legal move.
bool game_make_move(Game *game, Move move);

#endif
