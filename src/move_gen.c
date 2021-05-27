#include "move_gen.h"

#define PUSH_MOVE(moves, src_square, dst_square)                               \
	{                                                                      \
		(moves)->src = (src_square);                                   \
		(moves)->dst = (dst_square);                                   \
		(moves)++;                                                     \
	}

void gen_pawn_moves(Move **moves, Board *board, uint8_t square)
{
	static const int8_t PAWN_CAPTURES[] = { 9, 11 };

	int8_t dir = board->side_to_move == WHITE ? 1 : -1;

	{
		uint8_t dest_square = square + (dir * 10);
		uint8_t dest_piece = board->squares[dest_square];
		if (dest_piece != OFF_BOARD &&
		    PIECE_TYPE(dest_piece) == EMPTY) {
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
	if (SQUARE_TO_RANK(square) == (board->side_to_move == WHITE ? 1 : 6)) {
		uint8_t dest_square = square + (dir * 20);
		uint8_t dest_piece = board->squares[dest_square];
		if (dest_piece != OFF_BOARD &&
		    PIECE_TYPE(dest_piece) == EMPTY &&
		    PIECE_TYPE(board->squares[square + (dir * 10)]) == EMPTY) {
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
	for (size_t i = 0; i < sizeof(PAWN_CAPTURES); i++) {
		uint8_t dest_square = square + (dir * PAWN_CAPTURES[i]);
		uint8_t dest_piece = board->squares[dest_square];
		if (dest_piece == OFF_BOARD) {
			continue;
		}
		if (PIECE_TYPE(dest_piece) != EMPTY &&
		    PIECE_COLOR(dest_piece) != board->side_to_move) {
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_knight_moves(Move **moves, Board *board, uint8_t square)
{
	static const int8_t KNIGHT_MOVES[] = {
		8, 19, 21, 12, -8, -19, -21, -12
	};

	for (size_t i = 0; i < sizeof(KNIGHT_MOVES); i++) {
		uint8_t dest_square = square + KNIGHT_MOVES[i];
		uint8_t dest_piece = board->squares[dest_square];
		if (dest_piece == OFF_BOARD) {
			continue;
		}
		if (PIECE_TYPE(dest_piece) != EMPTY &&
		    PIECE_COLOR(dest_piece) == board->side_to_move) {
			continue;
		}
		PUSH_MOVE(*moves, square, dest_square);
	}
}

void gen_bishop_moves(Move **moves, Board *board, uint8_t square)
{
	static const int8_t BISHOP_DIRECTIONS[] = { 9, 11, -9, -11 };

	for (size_t i = 0; i < sizeof(BISHOP_DIRECTIONS); i++) {
		uint8_t dest_square = square;
		while (1) {
			dest_square += BISHOP_DIRECTIONS[i];
			uint8_t dest_piece = board->squares[dest_square];
			if (dest_piece == OFF_BOARD ||
			    (PIECE_TYPE(dest_piece) != EMPTY &&
			     PIECE_COLOR(dest_piece) == board->side_to_move)) {
				break;
			}
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_rook_moves(Move **moves, Board *board, uint8_t square)
{
	static const int8_t ROOK_DIRECTIONS[] = { 10, 1, -10, -1 };

	for (size_t i = 0; i < sizeof(ROOK_DIRECTIONS); i++) {
		uint8_t dest_square = square;
		while (1) {
			dest_square += ROOK_DIRECTIONS[i];
			uint8_t dest_piece = board->squares[dest_square];
			if (dest_piece == OFF_BOARD ||
			    (PIECE_TYPE(dest_piece) != EMPTY &&
			     PIECE_COLOR(dest_piece) == board->side_to_move)) {
				break;
			}
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_queen_moves(Move **moves, Board *board, uint8_t square)
{
	static const int8_t QUEEN_DIRECTIONS[] = { 9,  10,  11,	 1,
						   -9, -10, -11, -1 };

	for (size_t i = 0; i < sizeof(QUEEN_DIRECTIONS); i++) {
		uint8_t dest_square = square;
		while (1) {
			dest_square += QUEEN_DIRECTIONS[i];
			uint8_t dest_piece = board->squares[dest_square];
			if (dest_piece == OFF_BOARD ||
			    (PIECE_TYPE(dest_piece) != EMPTY &&
			     PIECE_COLOR(dest_piece) == board->side_to_move)) {
				break;
			}
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_king_moves(Move **moves, Board *board, uint8_t square)
{
	static const int8_t KING_MOVES[] = { 9, 10, 11, 1, -9, -10, -11, -1 };

	for (size_t i = 0; i < sizeof(KING_MOVES); i++) {
		uint8_t dest_square = square + KING_MOVES[i];
		uint8_t dest_piece = board->squares[dest_square];
		if (dest_piece == OFF_BOARD ||
		    (PIECE_TYPE(dest_piece) != EMPTY &&
		     PIECE_COLOR(dest_piece) == board->side_to_move)) {
			break;
		}
		PUSH_MOVE(*moves, square, dest_square);
	}
}

size_t gen_moves(Move *moves, Board *board)
{
	Move *initial_moves = moves;
	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			uint8_t square = RF(rank, file);
			uint8_t piece = board->squares[square];
			if (PIECE_COLOR(piece) != board->side_to_move) {
				continue;
			}
			switch (PIECE_TYPE(piece)) {
			case EMPTY:
				continue;
			case PAWN:
				gen_pawn_moves(&moves, board, square);
				break;
			case KNIGHT:
				gen_knight_moves(&moves, board, square);
				break;
			case BISHOP:
				gen_bishop_moves(&moves, board, square);
				break;
			case ROOK:
				gen_rook_moves(&moves, board, square);
				break;
			case QUEEN:
				gen_queen_moves(&moves, board, square);
				break;
			case KING:
				gen_king_moves(&moves, board, square);
				break;

			default:
				break;
			}
		}
	}
	return moves - initial_moves;
}
