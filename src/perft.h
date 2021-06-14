#ifndef _PERFT_H_
#define _PERFT_H_

#include "position.h"

#include <stddef.h>
#include <stdbool.h>

size_t perft(Position *position, size_t depth, bool print);

#endif
