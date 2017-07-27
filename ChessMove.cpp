#include "ChessMove.h"

#include <string>

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

	//Note we store the pieces when it's created. This since ChessBoardSquare->getPiece will be updated by ChessBoard->makeMove
	mAttacker = mSquareFrom->getPiece();
	if (mIsCapture) {
		mDefender = mSquareTo->getPiece();
	}
}

ChessBoardSquare * ChessMove::getSquareFrom()
{
	return mSquareFrom;
}

ChessBoardSquare * ChessMove::getSquareTo()
{
	return mSquareTo;
}

ChessPiece * ChessMove::getAttacker()
{
	return mAttacker;
}

ChessPiece * ChessMove::getDefender()
{
	return mDefender;
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

std::string ChessMove::toString()
{

	return mSquareFrom->toString() + mSquareTo->toString() + " isCapture" + std::to_string(isCapture());
}

