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

void gen_pawn_moves(Move **moves, Board *board, uint8_t square)
{
	int8_t dir = board->side_to_move == WHITE ? 1 : -1;

	{
		uint8_t dest_square = square + (dir * 10);
		uint8_t dest_piece = board->squares[dest_square];
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
	if (SQUARE_TO_RANK(square) == (board->side_to_move == WHITE ? 1 : 6)) {
		uint8_t dest_square = square + (dir * 20);
		uint8_t dest_piece = board->squares[dest_square];
		if (PIECE_TYPE(dest_piece) == EMPTY &&
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

		if ((dest_square == board->en_passant_target_square) ||
		    (PIECE_TYPE(dest_piece) != EMPTY &&
		     PIECE_COLOR(dest_piece) != board->side_to_move)) {
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

void gen_knight_moves(Move **moves, Board *board, uint8_t square)
{
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
	for (size_t i = 0; i < sizeof(BISHOP_DIRECTIONS); i++) {
		uint8_t dest_square = square;
		while (1) {
			dest_square += BISHOP_DIRECTIONS[i];
			uint8_t dest_piece = board->squares[dest_square];
			if (dest_piece == OFF_BOARD) {
				break;
			}
			if (PIECE_TYPE(dest_piece) != EMPTY) {
				if (PIECE_COLOR(dest_piece) !=
				    board->side_to_move) {
					PUSH_MOVE(*moves, square, dest_square);
				}
				break;
			}
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_rook_moves(Move **moves, Board *board, uint8_t square)
{
	for (size_t i = 0; i < sizeof(ROOK_DIRECTIONS); i++) {
		uint8_t dest_square = square;
		while (1) {
			dest_square += ROOK_DIRECTIONS[i];
			uint8_t dest_piece = board->squares[dest_square];
			if (dest_piece == OFF_BOARD) {
				break;
			}
			if (PIECE_TYPE(dest_piece) != EMPTY) {
				if (PIECE_COLOR(dest_piece) !=
				    board->side_to_move) {
					PUSH_MOVE(*moves, square, dest_square);
				}
				break;
			}
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_queen_moves(Move **moves, Board *board, uint8_t square)
{
	for (size_t i = 0; i < sizeof(QUEEN_DIRECTIONS); i++) {
		uint8_t dest_square = square;
		while (1) {
			dest_square += QUEEN_DIRECTIONS[i];
			uint8_t dest_piece = board->squares[dest_square];
			if (dest_piece == OFF_BOARD) {
				break;
			}
			if (PIECE_TYPE(dest_piece) != EMPTY) {
				if (PIECE_COLOR(dest_piece) !=
				    board->side_to_move) {
					PUSH_MOVE(*moves, square, dest_square);
				}
				break;
			}
			PUSH_MOVE(*moves, square, dest_square);
		}
	}
}

void gen_king_moves(Move **moves, Board *board, uint8_t square)
{
	for (size_t i = 0; i < sizeof(KING_MOVES); i++) {
		uint8_t dest_square = square + KING_MOVES[i];
		uint8_t dest_piece = board->squares[dest_square];
		if (dest_piece == OFF_BOARD ||
		    (PIECE_TYPE(dest_piece) != EMPTY &&
		     PIECE_COLOR(dest_piece) == board->side_to_move)) {
			continue;
		}
		PUSH_MOVE(*moves, square, dest_square);
	}
}

void gen_castling_moves(Move **moves, Board *board)
{
	if (board->side_to_move == WHITE) {
		if ((board->castling_ability & CASTLE_WHITE_KING) &&
		    board->squares[RF(0, 5)] == EMPTY &&
		    board->squares[RF(0, 6)] == EMPTY &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(0, 4)) &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(0, 5)) &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(0, 6))) {
			PUSH_MOVE(*moves, RF(0, 4), RF(0, 6));
		}
		if ((board->castling_ability & CASTLE_WHITE_QUEEN) &&
		    board->squares[RF(0, 1)] == EMPTY &&
		    board->squares[RF(0, 2)] == EMPTY &&
		    board->squares[RF(0, 3)] == EMPTY &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(0, 2)) &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(0, 3)) &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(0, 4))) {
			PUSH_MOVE(*moves, RF(0, 4), RF(0, 2));
		}
	}
	if (board->side_to_move == BLACK) {
		if ((board->castling_ability & CASTLE_BLACK_KING) &&
		    board->squares[RF(7, 5)] == EMPTY &&
		    board->squares[RF(7, 6)] == EMPTY &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(7, 4)) &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(7, 5)) &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(7, 6))) {
			PUSH_MOVE(*moves, RF(7, 4), RF(7, 6));
		}
		if ((board->castling_ability & CASTLE_BLACK_QUEEN) &&
		    board->squares[RF(7, 1)] == EMPTY &&
		    board->squares[RF(7, 2)] == EMPTY &&
		    board->squares[RF(7, 3)] == EMPTY &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(7, 2)) &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(7, 3)) &&
		    !is_square_threatened(board, board->side_to_move,
					  RF(7, 4))) {
			PUSH_MOVE(*moves, RF(7, 4), RF(7, 2));
		}
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
	gen_castling_moves(&moves, board);

	return moves - initial_moves;
}
