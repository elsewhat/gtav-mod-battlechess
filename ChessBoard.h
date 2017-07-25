#pragma once


#include "..\..\inc\types.h"
#include "ChessSide.h"
class ChessBoardSquare;
#include "ChessBoardSquare.h"
#include "ChessPiece.h"
class ChessSet;
#include "ChessSet.h"

#include <vector>
#include <array>


class ChessBoard {
public:
	ChessBoard(Vector3 baseLocation, float squareDeltaX, float squareDeltaY);

	bool hasSideChessPieceAt(ChessSide::Side side, int rank, int file);
	ChessPiece* getChessPieceAt(int rank, int file);

	void setWhiteChessSet(ChessSet* whiteSet);
	void setBlackChessSet(ChessSet* blackSet);

protected:
	Vector3 mBaseLocation;
	float mSquareDeltaX;
	float mSquareDeltaY;
	std::array<ChessBoardSquare*, 64>  mSquares;
	ChessSet* mWhiteSet;
	ChessSet* mBlackSet;

	void initializeSquares();
	ChessBoardSquare* getSquareAt(int rank, int file);
};
