#include "move.h"

#include <stdlib.h>

void make_move(Position *position, Move move)
{
	uint8_t moving_piece = position->squares[move.src];
	uint8_t captured_piece = position->squares[move.dst];

	// make move
	position->squares[move.dst] = moving_piece;
	position->squares[move.src] = EMPTY;

	// en passant
	if (PIECE_TYPE(moving_piece) == PAWN &&
	    move.dst == position->en_passant_target_square) {
		position->squares[RF(SQUARE_TO_RANK(move.src),
				     SQUARE_TO_FILE(move.dst))] = EMPTY;
	}

	// promotion
	if (PIECE_TYPE(moving_piece) == PAWN &&
	    (SQUARE_TO_RANK(move.dst) == 0 || SQUARE_TO_RANK(move.dst) == 7)) {
		position->squares[move.dst] =
			move.promotion_piece | PIECE_COLOR(moving_piece);
	}

	// castling
	if (PIECE_TYPE(moving_piece) == KING && abs(move.src - move.dst) == 2) {
		uint8_t rook_src_square =
			RF(PIECE_COLOR(moving_piece) == WHITE ? 0 : 7,
			   move.dst > move.src ? 7 : 0);
		uint8_t rook_dst_square = (move.dst + move.src) / 2;
		position->squares[rook_dst_square] =
			position->squares[rook_src_square];
		position->squares[rook_src_square] = EMPTY;
	}

	// remove castling ability
	if (PIECE_TYPE(moving_piece) == KING) {
		if (PIECE_COLOR(moving_piece) == WHITE) {
			position->castling_ability &= ~CASTLE_WHITE;
		} else {
			position->castling_ability &= ~CASTLE_BLACK;
		}
	}
	if (PIECE_TYPE(moving_piece) == ROOK) {
		if (PIECE_COLOR(moving_piece) == WHITE) {
			if (SQUARE_TO_FILE(move.src) == 0) {
				position->castling_ability &=
					~CASTLE_WHITE_QUEEN;
			} else if (SQUARE_TO_FILE(move.src) == 7) {
				position->castling_ability &=
					~CASTLE_WHITE_KING;
			}
		} else {
			if (SQUARE_TO_FILE(move.src) == 0) {
				position->castling_ability &=
					~CASTLE_BLACK_QUEEN;
			} else if (SQUARE_TO_FILE(move.src) == 7) {
				position->castling_ability &=
					~CASTLE_BLACK_KING;
			}
		}
	}
	if (PIECE_TYPE(captured_piece) == ROOK) {
		if (PIECE_COLOR(captured_piece) == WHITE) {
			if (SQUARE_TO_RANK(move.dst) == 0) {
				if (SQUARE_TO_FILE(move.dst) == 0) {
					position->castling_ability &=
						~CASTLE_WHITE_QUEEN;
				} else if (SQUARE_TO_FILE(move.dst) == 7) {
					position->castling_ability &=
						~CASTLE_WHITE_KING;
				}
			}
		} else {
			if (SQUARE_TO_RANK(move.dst) == 7) {
				if (SQUARE_TO_FILE(move.dst) == 0) {
					position->castling_ability &=
						~CASTLE_BLACK_QUEEN;
				} else if (SQUARE_TO_FILE(move.dst) == 7) {
					position->castling_ability &=
						~CASTLE_BLACK_KING;
				}
			}
		}
	}

	// flip side to move
	position->side_to_move ^= BLACK;

	// en passant target square
	if (PIECE_TYPE(moving_piece) == PAWN &&
	    abs(move.dst - move.src) == 20) {
		// if a pawn just made a two-square move
		position->en_passant_target_square =
			(move.dst + move.src) / 2; // square "behind" pawn
	} else {
		position->en_passant_target_square = 0;
	}

	// halfmove clock
	if (PIECE_TYPE(captured_piece) != EMPTY ||
	    PIECE_TYPE(moving_piece) == PAWN) {
		position->halfmove_clock = 0;
	} else {
		position->halfmove_clock++;
	}

	// fullmove counter
	if (position->side_to_move == WHITE) {
		// increment if black just made a move
		position->fullmove_counter++;
	}
}

char *move_to_string(Move move)
{
	// string can be up to 6 bytes long, including the null-byte
	char *string = malloc(6);

	string[0] = 'a' + SQUARE_TO_FILE(move.src);
	string[1] = '1' + SQUARE_TO_RANK(move.src);
	string[2] = 'a' + SQUARE_TO_FILE(move.dst);
	string[3] = '1' + SQUARE_TO_RANK(move.dst);
	switch (move.promotion_piece) {
	case EMPTY:
		string[4] = '\0';
		break;
	case QUEEN:
		string[4] = 'q';
		break;
	case ROOK:
		string[4] = 'r';
		break;
	case BISHOP:
		string[4] = 'b';
		break;
	case KNIGHT:
		string[4] = 'n';
		break;
	default:
		string[4] = '!';
		break;
	}
	string[5] = '\0';

	return string;
}

Move *move_from_string(const char *string)
{
	uint8_t src_file = string[0] - 'a';
	uint8_t src_rank = string[1] - '1';
	uint8_t dst_file = string[2] - 'a';
	uint8_t dst_rank = string[3] - '1';
	uint8_t promotion_piece;
	switch (string[4]) {
	case '\0':
		promotion_piece = 0;
		break;
	case 'q':
		promotion_piece = QUEEN;
		break;
	case 'r':
		promotion_piece = ROOK;
		break;
	case 'b':
		promotion_piece = BISHOP;
		break;
	case 'n':
		promotion_piece = KNIGHT;
		break;
	default:
		return NULL;
		break;
	}
	if (src_file > 7 || src_rank > 7 || dst_file > 7 || dst_rank > 7 ||
	    (string[4] != '\0' && string[5] != '\0')) {
		return NULL;
	}
	Move *move = malloc(sizeof(Move));
	move->src = RF(src_rank, src_file);
	move->dst = RF(dst_rank, dst_file);
	move->promotion_piece = promotion_piece;
	return move;
}
