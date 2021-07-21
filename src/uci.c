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

static void uci_printf_line(const char *line_format, ...)
{
	va_list argptr;
	va_start(argptr, line_format);
	vfprintf(stdout, line_format, argptr);
	va_end(argptr);
	fputc('\n', stdout);
	fflush(stdout);
	if (debug_fd != NULL) {
		fprintf(debug_fd, "write line: ");
		va_start(argptr, line_format);
		vfprintf(debug_fd, line_format, argptr);
		va_end(argptr);
		fputc('\n', debug_fd);
		fflush(debug_fd);
	}
}

static void uci_print_bestmove()
{
	uci_printf_line("bestmove %s", move_to_string(search.best_move));
}

static void command_position()
{
	if (search.running) {
		return;
	}
	char *token = strtok(NULL, " \n");
	if (token == NULL) {
		return;
	}
	if (strcmp(token, "startpos") == 0) {
		position = *fen_to_position(FEN_STARTING_POSITION);
	} else if (strcmp(token, "fen") == 0) {
		char *fen_string = strtok(NULL, " \n");
		if (fen_string == NULL) {
			return;
		}
		position = *fen_to_position(fen_string);
	} else {
		return;
	}
	token = strtok(NULL, " \n");
	if (token != NULL && strcmp(token, "moves") == 0) {
		while ((token = strtok(NULL, " \n")) != NULL) {
			Move *move = move_from_string(token);
			if (move == NULL) {
				continue;
			}
			make_move(&position, *move);
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
	search_init(&search, &position);
	search_start(&search);
	if (!infinite) {
		sleep(10);
		search_stop(&search);
		uci_print_bestmove();
	}
}

void uci()
{
	debug_fd = fopen("/tmp/uci.log", "w");
	if (debug_fd == NULL) {
		perror("error: could not open log file");
		exit(-1);
	}
	time_t t = time(NULL);
	fprintf(debug_fd, "%s\n", asctime(localtime(&t)));
	fflush(debug_fd);
	// wait for "uci" command
	{
		char *line = NULL;
		do {
			line = uci_read_line();
		} while (strcmp(line, "uci") != 0);
	}

	uci_printf_line("id name chess");
	uci_printf_line("id author Jonas Richard Keller");
	uci_printf_line("option name OwnBook type check default true");
	uci_printf_line("uciok");

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
			uci_printf_line("readyok");
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
