#include "board.h"
#include "fen.h"

int main(int argc, char const *argv[])
{
	board_print(fen_to_board(FEN_STARTING_POSITION));
}
