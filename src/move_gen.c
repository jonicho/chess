#include "move_gen.h"

#define PUSH_MOVE(moves, src_square, dst_square)                               \
	{                                                                      \
		(moves)->src = (src_square);                                   \
		(moves)->dst = (dst_square);                                   \
		(moves)->promotion_piece = EMPTY;                              \
		(moves)++;                                                     \
	}

#define PUSH_PROMOTION_MOVES(moves, src_square, dst_square)                    \
	{                                                                      \
		(moves)->src = (src_square);                                   \
		(moves)->dst = (dst_square);                                   \
		(moves)->promotion_piece = QUEEN;                              \
		(moves)++;                                                     \
		(moves)->src = (src_square);                                   \
		(moves)->dst = (dst_square);                                   \
		(moves)->promotion_piece = ROOK;                               \
		(moves)++;                                                     \
		(moves)->src = (src_square);                                   \
		(moves)->dst = (dst_square);                                   \
		(moves)->promotion_piece = BISHOP;                             \
		(moves)++;                                                     \
		(moves)->src = (src_square);                                   \
		(moves)->dst = (dst_square);                                   \
		(moves)->promotion_piece = KNIGHT;                             \
		(moves)++;                                                     \
	}

void gen_pawn_moves(Move **moves, const Position *position, uint8_t square,
		    bool captures_only)
{
	int8_t dir = position->side_to_move == WHITE ? 1 : -1;

	if (!captures_only) {
		{
			uint8_t dest_square = square + (dir * 10);
			uint8_t dest_piece = position->squares[dest_square];
			if (PIECE_TYPE(dest_piece) == EMPTY) {
				if (SQUARE_TO_RANK(dest_square) == 0 ||
				    SQUARE_TO_RANK(dest_square) == 7) {
					PUSH_PROMOTION_MOVES(*moves, square,
							     dest_square);
				} else {
					PUSH_MOVE(*moves, square, dest_square);
				}
			}
		}
		if (SQUARE_TO_RANK(square) ==
		    (position->side_to_move == WHITE ? 1 : 6)) {
			uint8_t dest_square = square + (dir * 20);
			uint8_t dest_piece = position->squares[dest_square];
			if (PIECE_TYPE(dest_piece) == EMPTY &&
			    PIECE_TYPE(
				    position->squares[square + (dir * 10)]) ==
				    EMPTY) {
				PUSH_MOVE(*moves, square, dest_square);
			}
		}
	}
	for (size_t i = 0; i < sizeof(PAWN_CAPTURES); i++) {
		uint8_t dest_square = square + (dir * PAWN_CAPTURES[i]);
		uint8_t dest_piece = position->squares[dest_square];
		if (dest_piece == OFF_BOARD) {
			continue;
		}

		if ((dest_square == position->en_passant_target_square) ||
		    (PIECE_TYPE(dest_piece) != EMPTY &&
		     PIECE_COLOR(dest_piece) != position->side_to_move)) {
			if (SQUARE_TO_RANK(dest_square) == 0 ||
			    SQUARE_TO_RANK(dest_square) == 7) {
				PUSH_PROMOTION_MOVES(*moves, square,
						     dest_square);
			} else {
				PUSH_MOVE(*moves, square, dest_square);
			}
		}
	}
}

void gen_knight_moves(Move **moves, const Position *position, uint8_t square,
		      bool captures_only)
{
	for (size_t i = 0; i < sizeof(KNIGHT_MOVES); i++) {
		uint8_t dest_square = square + KNIGHT_MOVES[i];
		uint8_t dest_piece = position->squares[dest_square];
		if (dest_piece == OFF_BOARD) {
			continue;
		}
		if (PIECE_TYPE(dest_piece) != EMPTY &&
		    PIECE_COLOR(dest_piece) == position->side_to_move) {
			continue;
		}
		if (PIECE_TYPE(dest_piece) == EMPTY && captures_only) {
			continue;
		}
		PUSH_MOVE(*moves, square, dest_square);
	}
}

void gen_bishop_moves(Move **moves, const Position *position, uint8_t square,
		      bool captures_only)
{
	for (size_t i = 0; i < sizeof(BISHOP_DIRECTIONS); i++) {
		uint8_t dest_square = square;
		while (1) {
			dest_square += BISHOP_DIRECTIONS[i];
			uint8_t dest_piece = position->squares[dest_square];
			if (dest_piece == OFF_BOARD) {
				break;
			}
			if (PIECE_TYPE(dest_piece) != EMPTY) {
				if (PIECE_COLOR(dest_piece) !=
				    position->side_to_move) {
					PUSH_MOVE(*moves, square, dest_square);
				}
				break;
			} else if (captures_only) {
				continue;
			}
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_rook_moves(Move **moves, const Position *position, uint8_t square,
		    bool captures_only)
{
	for (size_t i = 0; i < sizeof(ROOK_DIRECTIONS); i++) {
		uint8_t dest_square = square;
		while (1) {
			dest_square += ROOK_DIRECTIONS[i];
			uint8_t dest_piece = position->squares[dest_square];
			if (dest_piece == OFF_BOARD) {
				break;
			}
			if (PIECE_TYPE(dest_piece) != EMPTY) {
				if (PIECE_COLOR(dest_piece) !=
				    position->side_to_move) {
					PUSH_MOVE(*moves, square, dest_square);
				}
				break;
			} else if (captures_only) {
				continue;
			}
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_queen_moves(Move **moves, const Position *position, uint8_t square,
		     bool captures_only)
{
	for (size_t i = 0; i < sizeof(QUEEN_DIRECTIONS); i++) {
		uint8_t dest_square = square;
		while (1) {
			dest_square += QUEEN_DIRECTIONS[i];
			uint8_t dest_piece = position->squares[dest_square];
			if (dest_piece == OFF_BOARD) {
				break;
			}
			if (PIECE_TYPE(dest_piece) != EMPTY) {
				if (PIECE_COLOR(dest_piece) !=
				    position->side_to_move) {
					PUSH_MOVE(*moves, square, dest_square);
				}
				break;
			} else if (captures_only) {
				continue;
			}
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_king_moves(Move **moves, const Position *position, uint8_t square,
		    bool captures_only)
{
	for (size_t i = 0; i < sizeof(KING_MOVES); i++) {
		uint8_t dest_square = square + KING_MOVES[i];
		uint8_t dest_piece = position->squares[dest_square];
		if (dest_piece == OFF_BOARD ||
		    (PIECE_TYPE(dest_piece) != EMPTY &&
		     PIECE_COLOR(dest_piece) == position->side_to_move) ||
		    (PIECE_TYPE(dest_piece) == EMPTY && captures_only)) {
			continue;
		}
		PUSH_MOVE(*moves, square, dest_square);
	}
}

void gen_castling_moves(Move **moves, const Position *position)
{
	if (position->side_to_move == WHITE) {
		if ((position->castling_ability & CASTLE_WHITE_KING) &&
		    position->squares[RF(0, 5)] == EMPTY &&
		    position->squares[RF(0, 6)] == EMPTY &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(0, 4)) &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(0, 5)) &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(0, 6))) {
			PUSH_MOVE(*moves, RF(0, 4), RF(0, 6));
		}
		if ((position->castling_ability & CASTLE_WHITE_QUEEN) &&
		    position->squares[RF(0, 1)] == EMPTY &&
		    position->squares[RF(0, 2)] == EMPTY &&
		    position->squares[RF(0, 3)] == EMPTY &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(0, 2)) &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(0, 3)) &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(0, 4))) {
			PUSH_MOVE(*moves, RF(0, 4), RF(0, 2));
		}
	}
	if (position->side_to_move == BLACK) {
		if ((position->castling_ability & CASTLE_BLACK_KING) &&
		    position->squares[RF(7, 5)] == EMPTY &&
		    position->squares[RF(7, 6)] == EMPTY &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(7, 4)) &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(7, 5)) &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(7, 6))) {
			PUSH_MOVE(*moves, RF(7, 4), RF(7, 6));
		}
		if ((position->castling_ability & CASTLE_BLACK_QUEEN) &&
		    position->squares[RF(7, 1)] == EMPTY &&
		    position->squares[RF(7, 2)] == EMPTY &&
		    position->squares[RF(7, 3)] == EMPTY &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(7, 2)) &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(7, 3)) &&
		    !is_square_threatened(position, position->side_to_move,
					  RF(7, 4))) {
			PUSH_MOVE(*moves, RF(7, 4), RF(7, 2));
		}
	}
}

static void gen_moves_square(Move **moves, const Position *position,
			     uint8_t square, bool captures_only)
{
	uint8_t piece = position->squares[square];
	if (PIECE_COLOR(piece) != position->side_to_move) {
		return;
	}
	switch (PIECE_TYPE(piece)) {
	case EMPTY:
		return;
	case PAWN:
		gen_pawn_moves(moves, position, square, captures_only);
		break;
	case KNIGHT:
		gen_knight_moves(moves, position, square, captures_only);
		break;
	case BISHOP:
		gen_bishop_moves(moves, position, square, captures_only);
		break;
	case ROOK:
		gen_rook_moves(moves, position, square, captures_only);
		break;
	case QUEEN:
		gen_queen_moves(moves, position, square, captures_only);
		break;
	case KING:
		gen_king_moves(moves, position, square, captures_only);
		break;

	default:
		break;
	}
}

size_t gen_moves(Move *moves, const Position *position, bool captures_only)
{
	Move *initial_moves = moves;
	if (position->side_to_move == WHITE) {
		for (int rank = 7; rank >= 0; rank--) {
			for (int file = 0; file < 8; file++) {
				gen_moves_square(&moves, position,
						 RF(rank, file), captures_only);
			}
		}
	} else {
		for (int rank = 0; rank < 8; rank++) {
			for (int file = 0; file < 8; file++) {
				gen_moves_square(&moves, position,
						 RF(rank, file), captures_only);
			}
		}
	}
	if (!captures_only) {
		gen_castling_moves(&moves, position);
	}

	return moves - initial_moves;
}

size_t gen_legal_moves(Move *moves, const Position *position,
		       bool captures_only)
{
	size_t num_moves = gen_moves(moves, position, captures_only);
	size_t num_legal_moves = 0;
	for (size_t i = 0; i < num_moves; i++) {
		Position temp_position = *position;
		make_move(&temp_position, moves[i]);
		if (is_king_in_check(&temp_position,
				     BLACK ^ temp_position.side_to_move)) {
			continue;
		}
		moves[num_legal_moves++] = moves[i];
	}
	return num_legal_moves;
}
