#pragma once

#include "ChessBoardSquare.h";

class ChessMove {
public:
	ChessMove();
	ChessMove(ChessBoardSquare* squareFrom, ChessBoardSquare* squareTo, bool isCapture, bool isPromotion=false, bool isEnpassentCapture=false, bool isEnpassentEnabling=false);

	ChessBoardSquare* getSquareFrom();
	ChessBoardSquare* getSquareTo();

	bool isPromotion();
	bool isCapture();;
	bool isEnpassentCapture();
	bool isEnpassentEnabling();

protected:
	ChessBoardSquare* mSquareFrom;
	ChessBoardSquare* mSquareTo;
	bool mIsPromotion;
	bool mIsCapture;
	bool mIsEnpassentCapture;
	bool mIsEnpassentEnabling;
};