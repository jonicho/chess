#include "zobrist.h"

#include "pcg/pcg_basic.h"

#include <stddef.h>

uint64_t zobrist_piece_hashes[120][16];
uint64_t zobrist_black_to_move_hash;
uint64_t zobrist_castling_ability_hashes[16];
uint64_t zobrist_en_passant_target_square_hashes[8];

static uint64_t rand_uint64(pcg32_random_t *rng)
{
	return (uint64_t)pcg32_random_r(rng) << 32 | pcg32_random_r(rng);
}

void zobrist_init(unsigned int seed)
{
	pcg32_random_t rng;
	pcg32_srandom_r(&rng, seed, 0);
	for (size_t square = 0; square < 120; square++) {
		for (size_t piece = 0; piece < 16; piece++) {
			zobrist_piece_hashes[square][piece] = rand_uint64(&rng);
		}
	}
	zobrist_black_to_move_hash = rand_uint64(&rng);
	for (size_t file = 0; file < 8; file++) {
		zobrist_en_passant_target_square_hashes[file] = rand_uint64(&rng);
	}
	for (size_t i = 0; i < 16; i++) {
		zobrist_castling_ability_hashes[i] = rand_uint64(&rng);
	}
}

uint64_t zobrist_generate_hash(const Position *position)
{
	uint64_t hash = 0;

	for (size_t rank = 0; rank < 8; rank++) {
		for (size_t file = 0; file < 8; file++) {
			hash ^= zobrist_piece_hashes[RF(
				rank, file)][position->squares[RF(rank, file)]];
		}
	}
	if (position->side_to_move == BLACK) {
		hash ^= zobrist_black_to_move_hash;
	}
	hash ^= zobrist_castling_ability_hashes[position->castling_ability];
	if (position->en_passant_target_square != 0) {
		hash ^= zobrist_en_passant_target_square_hashes[SQUARE_TO_FILE(
			position->en_passant_target_square)];
	}
	return hash;
}
