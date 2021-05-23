#include "board.h"
#include "fen.h"

int main()
{
	board_print(fen_to_board(FEN_STARTING_POSITION));
}
