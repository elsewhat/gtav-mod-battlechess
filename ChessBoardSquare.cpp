#include "ChessBoardSquare.h"

#include "Utils.h"

ChessBoardSquare::ChessBoardSquare()
{
	mPiece = nullptr;
}


ChessBoardSquare::ChessBoardSquare(int file, int rank, bool isPromotion, Color color, Vector3 location, float headingWhite, float headingBlack)
{
	mSquareFile = file;
	mSquareRank = rank;
	mColor = color;
	mIsPromotion = isPromotion;
	mLocation = location;
	mHeadingWhite = headingWhite;
	mHeadingBlack = headingBlack;
}

int ChessBoardSquare::getSquareFile() const
{
	return mSquareFile;
}

int ChessBoardSquare::getSquareRank() const
{
	return mSquareRank;
}

ChessBoardSquare::Color ChessBoardSquare::getColor() const
{
	return mColor;
}

bool ChessBoardSquare::isEmpty() const
{
	if (!hasChessPiece) {
		return true;
	}
	else {
		return false;
	}
}

ChessPiece* ChessBoardSquare::getPiece() const
{
	return mPiece;
}

void ChessBoardSquare::setPiece(ChessPiece* piece)
{
	Logger::logDebug("ChessBoardSquare::setPiece");
	Logger::assert(mPiece != nullptr, "Already exist a piece for Square");

	mPiece = piece;
	mPiece->setLocation(mLocation);
	if (mPiece->getSide() == ChessSide::WHITE) {
		mPiece->setHeading(mHeadingWhite);
	}
	else {
		mPiece->setHeading(mHeadingBlack);
	}
	hasChessPiece = true;
}

void ChessBoardSquare::removePiece()
{
	mPiece = nullptr;
	hasChessPiece = false;
}

Vector3 ChessBoardSquare::getLocation() const
{
	return mLocation;
}

void ChessBoardSquare::setLocation(Vector3 location)
{
	mLocation = location;
}

boolean ChessBoardSquare::isPromotion() const
{
	return mIsPromotion;
}

float ChessBoardSquare::getHeadingWhite() const
{
	return mHeadingWhite;
}

float ChessBoardSquare::getHeadingBlack() const
{
	return mHeadingBlack;
}





