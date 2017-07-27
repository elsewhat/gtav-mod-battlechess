#pragma once

#include "ChessBoardSquare.h";
#include "ChessPiece.h";

class ChessMove {
public:
	ChessMove();
	ChessMove(ChessBoardSquare* squareFrom, ChessBoardSquare* squareTo, bool isCapture, bool isPromotion=false, bool isEnpassentCapture=false, bool isEnpassentEnabling=false, bool isCastling=false);

	ChessBoardSquare* getSquareFrom();
	ChessBoardSquare* getSquareTo();

	ChessPiece* getAttacker();
	ChessPiece* getDefender();

	bool isPromotion();
	bool isCapture();;
	bool isEnpassentCapture();
	bool isEnpassentEnabling();
	bool isCastling();

	bool isNull();
	std::string toString();
protected:
	bool mIsNull = true;
	ChessBoardSquare* mSquareFrom;
	ChessBoardSquare* mSquareTo;
	ChessPiece* mAttacker;
	ChessPiece* mDefender;
	bool mIsPromotion;
	bool mIsCapture;
	bool mIsEnpassentCapture;
	bool mIsEnpassentEnabling;
	bool mIsCastling;
};