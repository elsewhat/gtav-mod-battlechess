#include "ChessBoardSquare.h"

#include "inc\natives.h"
#include "inc\types.h"

#include "Utils.h"

ChessBoardSquare::ChessBoardSquare()
{
}


ChessBoardSquare::ChessBoardSquare(int file, int rank, bool isPromotion, bool isWhitePawnLine, bool isBlackPawnLine, Color color, Vector3 location, float headingWhite, float headingBlack)
{
	mSquareFile = file;
	mSquareRank = rank;
	mColor = color;
	mIsPromotion = isPromotion;
	mIsBlackPawnLine = isBlackPawnLine;
	mIsWhitePawnLine = isWhitePawnLine;
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
	Logger::assert(isEmpty(), "Already exist a piece for Square");

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
	mPiece = NULL;
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

boolean ChessBoardSquare::isPawnLine(ChessSide::Side side) const
{
	if (side == ChessSide::WHITE) {
		return mIsWhitePawnLine;
	}
	else {
		return mIsBlackPawnLine;
	}

}

boolean ChessBoardSquare::isEnpassentSquare(ChessSide::Side side) const
{
	if (side == ChessSide::WHITE) {
		return mCanWhiteEnpassentCapture;
	}
	else {
		return mCanBlackEnpassentCapture;
	}
}

bool ChessBoardSquare::setEnpassentSquare(ChessSide::Side side)
{
	if (side == ChessSide::WHITE) {
		mCanWhiteEnpassentCapture = true;
		mCanBlackEnpassentCapture = false;
	}
	else {
		mCanBlackEnpassentCapture = true;
		mCanWhiteEnpassentCapture = false;
	}
}

void ChessBoardSquare::removeEnpassentSquare()
{
	mCanBlackEnpassentCapture = false;
	mCanWhiteEnpassentCapture = false;
}


float ChessBoardSquare::getHeading(ChessSide::Side side) const
{
	if (side == ChessSide::WHITE) {
		return mHeadingWhite;
	}
	else {
		return mHeadingBlack;
	}
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

bool ChessBoardSquare::equals(ChessBoardSquare* square) const
{
	if (mSquareRank == square->getSquareRank() && mSquareFile == square->getSquareFile()) {
		return true;
	}
	else {
		return false;
	}
}

void ChessBoardSquare::drawOnTick()
{

	if (mHighlightAsSelected) {
		int highlightColorR = 192;
		int highlightColorG = 192;
		int highlightColorB = 192;
		GRAPHICS::DRAW_SPOT_LIGHT(mLocation.x, mLocation.y, mLocation.z + 20.0f, 0, 0, -1.0, highlightColorR, highlightColorG, highlightColorB, 100.0f, 1.0, 0.0f, 4.0f, 1.0f);
	}else if (mHighlightAsCursor) {
		int highlightColorR = 255;
		int highlightColorG = 255;
		int highlightColorB = 255;
		GRAPHICS::DRAW_SPOT_LIGHT(mLocation.x, mLocation.y, mLocation.z + 20.0f, 0, 0, -1.0, highlightColorR, highlightColorG, highlightColorB, 100.0f, 1.0, 0.0f, 4.0f, 1.0f);
	}
	else if (mHighlightAsPossible){
		int highlightColorR = 0;
		int highlightColorG = 128;
		int highlightColorB = 0;
		GRAPHICS::DRAW_SPOT_LIGHT(mLocation.x, mLocation.y, mLocation.z + 20.0f, 0, 0, -1.0, highlightColorR, highlightColorG, highlightColorB, 100.0f, 1.0, 0.0f, 4.0f, 1.0f);
	}
}

std::string ChessBoardSquare::toString()
{
	return std::to_string(mSquareRank) + "" + std::to_string(mSquareFile);
}



