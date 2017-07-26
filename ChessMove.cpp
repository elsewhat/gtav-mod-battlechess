#include "ChessMove.h"

ChessMove::ChessMove()
{
	mSquareFrom = NULL;
	mSquareTo = NULL;
}

ChessMove::ChessMove(ChessBoardSquare * squareFrom, ChessBoardSquare * squareTo, bool isCapture, bool isPromotion, bool isEnpassentCapture, bool isEnpassentEnabling)
{
	mSquareFrom = squareFrom;
	mSquareTo = squareTo;
	mIsCapture = isCapture;
	mIsPromotion = isPromotion;
	mIsEnpassentCapture = isEnpassentCapture;
	mIsEnpassentEnabling = isEnpassentEnabling;
}

ChessBoardSquare * ChessMove::getSquareFrom()
{
	return mSquareFrom;
}

ChessBoardSquare * ChessMove::getSquareTo()
{
	return mSquareTo;
}

bool ChessMove::isPromotion()
{
	return mIsPromotion;
}

bool ChessMove::isCapture()
{
	return mIsCapture;
}

bool ChessMove::isEnpassentCapture()
{
	return mIsEnpassentCapture;
}

bool ChessMove::isEnpassentEnabling()
{
	return mIsEnpassentEnabling;
}

