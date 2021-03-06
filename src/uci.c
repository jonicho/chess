#include "uci.h"

#include "position.h"
#include "move.h"
#include "fen.h"
#include "search.h"
#include "zobrist.h"
#include "table.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>

static Position position;
static Search search;
static FILE *debug_fd;

static char *uci_read_line()
{
	char *line = NULL;
	size_t n = 0;
	ssize_t nread = getline(&line, &n, stdin);
	if (nread == -1) {
		perror("error: could not read line");
		exit(-1);
	}
	line[--nread] = '\0'; // remove newline
	if (debug_fd != NULL) {
		fprintf(debug_fd, "read line: %s\n", line);
		fflush(debug_fd);
	}
	return line;
}

static void uci_printf(const char *line_format, ...)
{
	va_list argptr;
	va_start(argptr, line_format);
	vfprintf(stdout, line_format, argptr);
	va_end(argptr);
	fflush(stdout);
	if (debug_fd != NULL) {
		va_start(argptr, line_format);
		vfprintf(debug_fd, line_format, argptr);
		va_end(argptr);
		fflush(debug_fd);
	}
}

static void uci_print_bestmove()
{
	uci_printf("bestmove %s\n", move_to_string(search.best_move));
}

static void uci_print_info(const Search *search)
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

	uci_printf("info depth %d time %ld nodes %ld nps %ld score cp %d pv",
		   search->depth, elapsed_millis, search->nodes, nps,
		   search->best_move_eval);

	Move *pv = malloc(sizeof(Move) * search->depth);
	size_t pv_length = table_get_pv(search->position, search->depth, pv);
	for (size_t i = 0; i < pv_length; i++) {
		uci_printf(" %s", move_to_string(pv[i]));
	}
	uci_printf("\n");
}

static void command_position()
{
	if (search_is_running(&search)) {
		return;
	}
	char *token = strtok(NULL, " \n");
	if (token == NULL) {
		return;
	}
	if (strcmp(token, "startpos") == 0) {
		position = *fen_to_position(FEN_STARTING_POSITION);
	} else if (strcmp(token, "fen") == 0) {
		char *tokens[6];
		for (size_t i = 0; i < sizeof(tokens) / sizeof(tokens[0]);
		     i++) {
			tokens[i] = strtok(NULL, " \n");
		}
		char fen_string[256];
		snprintf(fen_string, sizeof(fen_string), "%s %s %s %s %s %s",
			 tokens[0], tokens[1], tokens[2], tokens[3], tokens[4],
			 tokens[5]);

		Position *position_ptr = fen_to_position(fen_string);
		if (position_ptr == NULL) {
			return;
		}
		position = *position_ptr;
	} else {
		return;
	}
	token = strtok(NULL, " \n");
	if (token != NULL && strcmp(token, "moves") == 0) {
		while ((token = strtok(NULL, " \n")) != NULL) {
			Move move = move_from_string(token);
			if (IS_NO_MOVE(move)) {
				continue;
			}
			make_move(&position, move);
		}
	}
}

static void command_go()
{
	char *token = strtok(NULL, " \n");
	if (token == NULL) {
		return;
	}
	bool infinite = false;
	if (strcmp(token, "infinite") == 0) {
		infinite = true;
	}
	search_init(&search, &position, uci_print_info);
	search_start(&search);
	if (!infinite) {
		sleep(10);
		search_stop(&search);
		uci_print_bestmove();
	}
}

void uci()
{
#ifdef DEBUG
	debug_fd = fopen("/tmp/uci.log", "w");
	if (debug_fd == NULL) {
		perror("error: could not open log file");
		exit(-1);
	}
	time_t t = time(NULL);
	fprintf(debug_fd, "%s\n", asctime(localtime(&t)));
	fflush(debug_fd);
#endif
	// wait for "uci" command
	{
		char *line = NULL;
		do {
			line = uci_read_line();
		} while (strcmp(line, "uci") != 0);
	}

	uci_printf("id name chess\n");
	uci_printf("id author Jonas Richard Keller\n");
	uci_printf("option name OwnBook type check default true\n");
	uci_printf("uciok\n");

	zobrist_init(time(NULL));
	table_init();

	while (true) {
		char *line = uci_read_line();
		char *command_name = strtok(line, " \n");
		if (command_name == NULL) {
			free(line);
			continue;
		}
		if (strcmp(command_name, "isready") == 0) {
			uci_printf("readyok\n");
		} else if (strcmp(command_name, "position") == 0) {
			command_position();
		} else if (strcmp(command_name, "go") == 0) {
			command_go();
		} else if (strcmp(command_name, "stop") == 0) {
			search_stop(&search);
			uci_print_bestmove();
		} else if (strcmp(command_name, "quit") == 0) {
			exit(0);
		}
		free(line);
	}
}
