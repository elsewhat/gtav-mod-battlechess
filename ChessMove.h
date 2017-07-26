#pragma once

#include "ChessBoardSquare.h";

class ChessMove {
public:
	ChessMove();
	ChessMove(ChessBoardSquare* squareFrom, ChessBoardSquare* squareTo, bool isCapture, bool isPromotion=false, bool isEnpassentCapture=false, bool isEnpassentEnabling=false, bool isCastling=false);

	ChessBoardSquare* getSquareFrom();
	ChessBoardSquare* getSquareTo();

	bool isPromotion();
	bool isCapture();;
	bool isEnpassentCapture();
	bool isEnpassentEnabling();
	bool isCastling();

	bool isNull();
protected:
	bool mIsNull = true;
	ChessBoardSquare* mSquareFrom;
	ChessBoardSquare* mSquareTo;
	bool mIsPromotion;
	bool mIsCapture;
	bool mIsEnpassentCapture;
	bool mIsEnpassentEnabling;
	bool mIsCastling;
};