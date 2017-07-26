#include "ChessMove.h"

ChessMove::ChessMove()
{
	mSquareFrom = NULL;
	mSquareTo = NULL;
}

ChessMove::ChessMove(ChessBoardSquare * squareFrom, ChessBoardSquare * squareTo, bool isCapture, bool isPromotion)
{
	mSquareFrom = squareFrom;
	mSquareTo = squareTo;
	mIsCapture = isCapture;
	mIsPromotion = isPromotion;
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

