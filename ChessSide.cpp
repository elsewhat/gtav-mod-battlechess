#include "ChessSide.h"

#include "Utils.h"

ChessSide::Side ChessSide::oppositeSide(ChessSide::Side side)
{
	if (side == WHITE) {
		return BLACK;
	}
	else {
		return WHITE;
	}
}
