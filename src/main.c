#include "position.h"
#include "fen.h"
#include "perft.h"
#include "test.h"
#include "game.h"
#include "search.h"
#include "zobrist.h"
#include "table.h"
#include "uci.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SEARCH_SECONDS 10

void play_game()
{
	zobrist_init(time(NULL));
	table_init();

	Game game;
	game_init(&game);
	while (true) {
		position_print_board(game.current_position);

		if (game.outcome != OUTCOME_NONE) {
			printf("The game ended: ");
			switch (game.outcome) {
			case OUTCOME_NONE:
				fprintf(stderr, "error: unreachable\n");
				exit(-1);
			case OUTCOME_WHITE_WON:
				printf("White won!\n");
				break;
			case OUTCOME_BLACK_WON:
				printf("Black won!\n");
				break;
			case OUTCOME_DRAW:
				printf("Draw!\n");
				break;
			}
			return;
		}

		if (game.current_position->side_to_move == BLACK) {
			printf("\nIt's %s's turn. Searching for %d seconds...\n",
			       game.current_position->side_to_move == WHITE ?
					     "white" :
					     "black",
			       SEARCH_SECONDS);
			Search search;
			search_init(&search, game.current_position);
			search_start(&search);
			sleep(SEARCH_SECONDS);
			search_stop(&search);
			printf("Search completed: depth: %ld, nodes searched: %ld, nps: %ld, best move: %s, best move eval: %d",
			       search.depth, search.nodes,
			       search.nodes / SEARCH_SECONDS,
			       move_to_string(search.best_move),
			       search.best_move_eval);

			if (search.best_move_eval >= CHECKMATE_EVAL) {
				int moves_until_win =
					(-search.best_move_eval +
					 CHECKMATE_EVAL + (int)search.depth) /
						2 +
					1;
				printf(" (winning in %d move%s)\n",
				       moves_until_win,
				       moves_until_win == 1 ? "" : "s");
			} else if (search.best_move_eval <= -CHECKMATE_EVAL) {
				int moves_until_lose =
					(search.best_move_eval +
					 CHECKMATE_EVAL + (int)search.depth) /
					2;
				printf(" (losing in %d move%s)\n",
				       moves_until_lose,
				       moves_until_lose == 1 ? "" : "s");
			} else {
				printf("\n");
			}
			Move *pv = malloc(sizeof(Move) * search.depth);
			size_t pv_length = table_get_pv(game.current_position,
							search.depth, pv);
			printf("pv: ");
			for (size_t i = 0; i < pv_length; i++) {
				printf("%s ", move_to_string(pv[i]));
			}
			printf("\n");

			if (!game_make_move(&game, search.best_move)) {
				fprintf(stderr,
					"error: computer made an illegal move\n");
				exit(-1);
			}
			continue;
		}
		printf("\nIt's %s's turn. Make a move: ",
		       game.current_position->side_to_move == WHITE ? "white" :
									    "black");

		bool invalid_move;
		do {
			invalid_move = false;
			size_t n = 0;
			char *line = NULL;
			ssize_t nread = getline(&line, &n, stdin);

			if (nread == -1) {
				perror("error: could not read line");
				exit(-1);
			}
			line[--nread] = '\0'; // remove trailing newline
			Move *move = move_from_string(line);
			if (move == NULL || !game_make_move(&game, *move)) {
				invalid_move = true;
				printf("Invalid move, try again: ");
			}
		} while (invalid_move);
	}
}

int main(int argc, char const *argv[])
{
	if (argc > 2 && (strcmp(argv[1], "perft") == 0)) {
		perft(fen_to_position(argc > 3 ? argv[3] :
						       FEN_STARTING_POSITION),
		      atol(argv[2]), true);
		exit(0);
	} else if (argc > 1 && (strcmp(argv[1], "test") == 0)) {
		int test_result = test();
		exit(test_result);
	} else if (argc > 1 && (strcmp(argv[1], "play") == 0)) {
		play_game();
	}

	uci();
}
