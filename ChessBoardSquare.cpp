#include "ChessBoardSquare.h"

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"

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

void ChessBoardSquare::drawSpotAbove(int colorR, int colorG, int colorB) const {

}

bool ChessBoardSquare::doHighlightAsSelected() const
{
	return mHighlightAsSelected;
}

bool ChessBoardSquare::doHighlightAsPossible() const
{
	return mHighlightAsPossible;
}

void ChessBoardSquare::setDoHighlightAsPossible(bool highlightAsPossible)
{
	mHighlightAsPossible = highlightAsPossible;
}

void ChessBoardSquare::setDoHighlightAsSelected(bool highlightAsSelected)
{
	mHighlightAsSelected = highlightAsSelected;
}

bool ChessBoardSquare::doHighlightAsCursor() const
{
	return mHighlightAsCursor;
}

void ChessBoardSquare::setDoHighlightAsCursor(bool highlightAsCursor)
{
	mHighlightAsCursor = highlightAsCursor;
}

void ChessBoardSquare::drawOnTick()
{
	if (mHighlightAsCursor) {
		int highlightColorR = 255;
		int highlightColorG = 255;
		int highlightColorB = 255;
		GRAPHICS::DRAW_SPOT_LIGHT(mLocation.x, mLocation.y, mLocation.z + 20.0f, 0, 0, -1.0, highlightColorR, highlightColorG, highlightColorB, 100.0f, 1.0, 0.0f, 4.0f, 1.0f);
	}
	else if (mHighlightAsSelected) {
		int highlightColorR = 192;
		int highlightColorG = 192;
		int highlightColorB = 192;
		GRAPHICS::DRAW_SPOT_LIGHT(mLocation.x, mLocation.y, mLocation.z + 20.0f, 0, 0, -1.0, highlightColorR, highlightColorG, highlightColorB, 100.0f, 1.0, 0.0f, 4.0f, 1.0f);
	}
	else if (mHighlightAsPossible){
		int highlightColorR = 0;
		int highlightColorG = 128;
		int highlightColorB = 0;
		GRAPHICS::DRAW_SPOT_LIGHT(mLocation.x, mLocation.y, mLocation.z + 20.0f, 0, 0, -1.0, highlightColorR, highlightColorG, highlightColorB, 100.0f, 1.0, 0.0f, 4.0f, 1.0f);
	}
}



