#ifndef _ZOBRIST_H_
#define _ZOBRIST_H_

#include "position.h"
#include "move.h"

#include <stdint.h>

extern uint64_t zobrist_piece_hashes[120][16];
extern uint64_t zobrist_black_to_move_hash;
extern uint64_t zobrist_castling_ability_hashes[16];
extern uint64_t zobrist_en_passant_target_square_hashes[8];

void zobrist_init(unsigned int seed);

uint64_t zobrist_generate_hash(const Position *position);

#endif
