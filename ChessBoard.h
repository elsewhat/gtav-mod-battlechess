#pragma once


#include "..\..\inc\types.h"
#include "ChessSide.h"
class ChessBoardSquare;
#include "ChessBoardSquare.h"
#include "ChessPiece.h"
class ChessSet;
#include "ChessSet.h"
class ChessMove;
#include "ChessMove.h"

#include <vector>
#include <array>


class ChessBoard {
public:
	ChessBoard(Vector3 baseLocation, float squareDeltaX, float squareDeltaY);

	ChessSide::Side sideToMove();
	void makeMove(ChessMove* chessMove);

	bool hasSideChessPieceAt(ChessSide::Side side, int rank, int file);
	ChessPiece* getChessPieceAt(int rank, int file);
	ChessBoardSquare* getSquareAt(int rank, int file);
	bool isValidRankAndFile(int rank, int file);

	std::vector<ChessMove> possibleMoves(ChessSide::Side side, ChessBoardSquare* squareFrom);

	void setWhiteChessSet(ChessSet* whiteSet);
	void setBlackChessSet(ChessSet* blackSet);

	void spawnChessPieces();

	void drawOnTick();
protected:
	ChessSide::Side mSideToMove;
	std::array<ChessBoardSquare*, 64>  mSquares;
	ChessSet* mWhiteSet;
	ChessSet* mBlackSet;

	Vector3 mBaseLocation;
	float mSquareDeltaX;
	float mSquareDeltaY;


	void initializeSquares();

	std::vector<ChessMove> generatePawnMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare* squareFrom);
};
