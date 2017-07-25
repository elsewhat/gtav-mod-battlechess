#include "ChessBoard.h"

#include "..\..\inc\natives.h"
#include "Utils.h"


ChessBoard::ChessBoard(Vector3 baseLocation, float squareDeltaX, float squareDeltaY)
{
	mBaseLocation = baseLocation;
	mSquareDeltaX = squareDeltaX;
	mSquareDeltaY = squareDeltaY;
	initializeSquares();
}

void ChessBoard::setWhiteChessSet(ChessSet* whiteSet)
{
	mWhiteSet = whiteSet;
}

void ChessBoard::setBlackChessSet(ChessSet* blackSet)
{
	mBlackSet = blackSet;
}

/*
Create square in the following pattern
Double-digit values are indexs in array
A  B  C  D  E  F  G  H
File value                Rank value
1  2  3  4  5  6  7  8
00 01 02 03 04 05 06 07    1
08 09 10 11 12 13 14 15    2
16 17 18 19 20 21 22 23    3
24 25 26 27 28 29 30 31    4
32 33 34 35 36 37 38 39    5
40 41 42 43 44 45 46 47    6
48 49 50 51 52 53 54 55    7
56 57 58 59 60 61 62 63    8
*/
void ChessBoard::initializeSquares()
{
	int index = 0;
	float headingWhite = 1.0;
	float headingBlack = 0.0;

	for (int rank = 1; rank <= 8; rank++) {
		bool isPromotion = false;
		if (rank == 1 || rank == 8) {
			isPromotion = true;
		}

		for (int file = 1; file <= 8; file++) {
			//A1 is Dark. Alternating afterwards
			ChessBoardSquare::Color color = ChessBoardSquare::DARK;
			if (rank + file % 2 == 1) {
				color = ChessBoardSquare::LIGHT;
			}

			Vector3 location;
			location.x = mBaseLocation.x + mSquareDeltaX * (file-1) ;
			location.y = mBaseLocation.y + mSquareDeltaY * (rank-1);
			location.z = mBaseLocation.z;

			mSquares[index] = new ChessBoardSquare(file, rank, isPromotion, color, location, headingWhite, headingBlack);
			index++;
		}
	}

}
