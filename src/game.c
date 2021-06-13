#include "game.h"

#include "fen.h"
#include "move_gen.h"

#include <stdio.h>
#include <stdlib.h>

void game_init(Game *game)
{
	game->current_position = fen_to_position(FEN_STARTING_POSITION);
	game->moves_capacity = INITIAL_MOVES_CAPACITY;
	game->num_moves = 0;
	game->moves = malloc(sizeof(Move) * game->moves_capacity);
}

bool game_make_move(Game *game, Move move)
{
	Move moves[MAX_NUM_PSEUDO_LEGAL_MOVES];
	size_t num_moves = gen_legal_moves(moves, game->current_position);
	bool is_move_legal = false;

	for (size_t i = 0; i < num_moves; i++) {
		if (moves[i].src == move.src && moves[i].dst == move.dst &&
		    moves[i].promotion_piece == move.promotion_piece) {
			is_move_legal = true;
			break;
		}
	}

	if (!is_move_legal) {
		return false;
	}

	Position temp_position = *game->current_position;

	make_move(&temp_position, move);

	if (is_king_in_check(&temp_position,
			     BLACK ^ temp_position.side_to_move)) {
		return false;
	}

	*game->current_position = temp_position;

	while (game->num_moves >= game->moves_capacity) {
		game->moves_capacity *= 2;
		game->moves = realloc(game->moves,
				      sizeof(Move) * game->moves_capacity);
	}
	game->moves[game->num_moves++] = move;

	return true;
}
