#pragma once


//TODO: Do we really need a class for this ? leave it for now in order to be in line with CuteChess
class ChessSide {
public:
	enum Side {
		WHITE,
		BLACK
	};

	static Side oppositeSide(Side side);

};