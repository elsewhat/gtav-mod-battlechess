#include "ChessMove.h"

ChessMove::ChessMove()
{
	mSquareFrom = NULL;
	mSquareTo = NULL;
	mIsNull = true;
}

ChessMove::ChessMove(ChessBoardSquare * squareFrom, ChessBoardSquare * squareTo, bool isCapture, bool isPromotion, bool isEnpassentCapture, bool isEnpassentEnabling,bool isCastling)
{
	mSquareFrom = squareFrom;
	mSquareTo = squareTo;
	mIsCapture = isCapture;
	mIsPromotion = isPromotion;
	mIsEnpassentCapture = isEnpassentCapture;
	mIsEnpassentEnabling = isEnpassentEnabling;
	mIsCastling = isCastling;
	mIsNull = false;
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

bool ChessMove::isCastling()
{
	return mIsCastling;
}

bool ChessMove::isNull()
{
	return mIsNull;
}

