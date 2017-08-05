#pragma once


#include "inc\types.h"
#include "ChessSide.h"
class ChessBoardSquare;
#include "ChessBoardSquare.h"
class ChessPiece;
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
	void makeMove(ChessMove chessMove);

	bool hasSideChessPieceAt(ChessSide::Side side, int rank, int file);
	ChessPiece* getChessPieceAt(int rank, int file);
	ChessBoardSquare* getSquareAt(int rank, int file);
	bool isValidRankAndFile(int rank, int file);

	std::vector<ChessMove> possibleMoves(ChessSide::Side side, ChessBoardSquare* squareFrom);

	void setWhiteChessSet(ChessSet* whiteSet);
	void setBlackChessSet(ChessSet* blackSet);

	void spawnChessPieces();

	void freezeAllExcept(std::vector<ChessPiece*> chessPieces);


	void drawOnTick();
protected:
	ChessSide::Side mSideToMove;
	std::array<ChessBoardSquare*, 64>  mSquares;
	ChessSet* mWhiteSet;
	ChessSet* mBlackSet;

	Vector3 mBaseLocation;
	float mSquareDeltaX;
	float mSquareDeltaY;

	Hash mWhiteRelationshipGroupHash;
	Hash mBlackRelationshipGroupHash;


	void initializeSquares();

	std::vector<ChessMove> generatePawnMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare* squareFrom, ChessPiece* piece);
	std::vector<ChessMove> generateKingMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare* squareFrom, ChessPiece* piece);
	std::vector<ChessMove> generateSlidingMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare* squareFrom, ChessPiece* piece);
	std::vector<ChessMove> generateHoppingMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare* squareFrom, ChessPiece* piece);
	std::vector<ChessMove> generateCastlingMoves(std::vector<ChessMove> possibleMoves, ChessSide::Side side, ChessBoardSquare* squareFrom, ChessPiece* piece);
	
};
