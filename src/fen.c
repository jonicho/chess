#include "fen.h"

#include "zobrist.h"

#include <stdlib.h>

uint8_t char_to_piece_code(char c)
{
	uint8_t piece_code = (c & (1 << 5)) ? BLACK : WHITE;
	c = c & ~(1 << 5); // to uppercase
	switch (c) {
	case 'P':
		piece_code |= PAWN;
		break;
	case 'N':
		piece_code |= KNIGHT;
		break;
	case 'B':
		piece_code |= BISHOP;
		break;
	case 'R':
		piece_code |= ROOK;
		break;
	case 'Q':
		piece_code |= QUEEN;
		break;
	case 'K':
		piece_code |= KING;
		break;
	default:
		return 255;
	}
	return piece_code;
}

Position *fen_to_position(const char *fen)
{
	Position *position = malloc(sizeof(Position));
	position_init(position);
	int rank = 7;
	int file = 0;
	int i = 0;
	char c;

	// piece placement
	while ((c = fen[i++]) != ' ') {
		if ((c == '\0') || (file >= 8 && c != '/') || (rank < 0)) {
			free(position);
			return NULL;
		} else if (c == '/') {
			rank--;
			file = 0;
		} else if (c == ' ') {
			break;
		} else if (c >= '1' && c <= '8') {
			file += c - '0';
		} else {
			uint8_t piece_code = char_to_piece_code(c);
			if (piece_code == 255) {
				free(position);
				return NULL;
			}
			position->squares[RF(rank, file++)] = piece_code;
		}
	}

	// side to move
	switch (fen[i++]) {
	case 'w':
		position->side_to_move = WHITE;
		break;
	case 'b':
		position->side_to_move = BLACK;
		break;
	default:
		free(position);
		return NULL;
	}
	if (fen[i++] != ' ') {
		free(position);
		return NULL;
	}

	// castling ability
	if (fen[i] == '-') {
		i++;
		if (fen[i++] != ' ') {
			free(position);
			return NULL;
		}
	} else {
		while ((c = fen[i++]) != ' ') {
			uint8_t castling_ability_to_add;
			switch (c) {
			case 'K':
				castling_ability_to_add = CASTLE_WHITE_KING;
				break;
			case 'Q':
				castling_ability_to_add = CASTLE_WHITE_QUEEN;
				break;
			case 'k':
				castling_ability_to_add = CASTLE_BLACK_KING;
				break;
			case 'q':
				castling_ability_to_add = CASTLE_BLACK_QUEEN;
				break;

			default:
				free(position);
				return NULL;
			}
			if (position->castling_ability &
			    castling_ability_to_add) {
				free(position);
				return NULL;
			}
			position->castling_ability |= castling_ability_to_add;
		}
	}

	// en passant target square
	c = fen[i++];
	if (c >= 'a' && c <= 'h' && (fen[i] == '3' || fen[i] == '6')) {
		position->en_passant_target_square = RF(fen[i] - '1', c - 'a');
		i++;
	} else if (c != '-') {
		free(position);
		return NULL;
	}
	if (fen[i++] != ' ') {
		free(position);
		return NULL;
	}

	// halfmove clock
	while ((c = fen[i++]) != ' ') {
		if (c >= '0' && c <= '9') {
			position->halfmove_clock *= 10;
			position->halfmove_clock += c - '0';
		} else {
			free(position);
			return NULL;
		}
	}

	// fullmove counter
	if (fen[i] == '0') {
		free(position);
		return NULL;
	}

	while ((c = fen[i++]) != '\0') {
		if (c >= '0' && c <= '9') {
			position->fullmove_counter *= 10;
			position->fullmove_counter += c - '0';
		} else {
			free(position);
			return NULL;
		}
	}

	position->hash = zobrist_generate_hash(position);

	return position;
}
