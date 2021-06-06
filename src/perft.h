#ifndef _PERFT_H_
#define _PERFT_H_

#include "board.h"

#include <stddef.h>
#include <stdbool.h>

size_t perft(const Board *board, size_t depth, bool print);

#endif
