#pragma once

#include "ChessBoardSquare.h";
#include "ChessPiece.h";

class ChessMove {
public:
	ChessMove();
	ChessMove(ChessBoardSquare* squareFrom, ChessBoardSquare* squareTo, bool isCapture, bool isPromotion);

	ChessBoardSquare* getSquareFrom();
	ChessBoardSquare* getSquareTo();

	bool isPromotion();
	bool isCapture();;

protected:
	ChessBoardSquare* mSquareFrom;
	ChessBoardSquare* mSquareTo;
	bool mIsPromotion;
	bool mIsCapture;
};