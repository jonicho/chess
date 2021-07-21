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
#include <sys/time.h>

#define SEARCH_SECONDS 10
#define EVAL_STRING_LENGTH 256

static void print_search_info(const Search *search)
{
	struct timeval time;
	if (search->stop_time.tv_sec != 0) {
		time = search->stop_time;
	} else {
		gettimeofday(&time, NULL);
	}
	size_t elapsed_millis = (time.tv_sec * 1000 + time.tv_usec / 1000) -
				(search->start_time.tv_sec * 1000 +
				 search->start_time.tv_usec / 1000);
	size_t nps = search->nodes / (elapsed_millis / 1000.0);
	char eval_string[EVAL_STRING_LENGTH];
	search_eval_to_string(search, eval_string, EVAL_STRING_LENGTH);
	printf("search: depth: %2ld, time: %7ld, nodes: %12ld, nps: %8ld, best move eval: %6s, pv:",
	       search->depth, elapsed_millis, search->nodes, nps, eval_string);

	Move *pv = malloc(sizeof(Move) * search->depth);
	size_t pv_length = table_get_pv(search->position, search->depth, pv);
	for (size_t i = 0; i < pv_length; i++) {
		printf(" %s", move_to_string(pv[i]));
	}
	printf("\n");
}

static void play_game()
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
			search_init(&search, game.current_position,
				    print_search_info);
			search_start(&search);
			sleep(SEARCH_SECONDS);
			search_stop(&search);
			printf("search finished\n");
			print_search_info(&search);

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
