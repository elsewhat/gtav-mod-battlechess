#include "ChessBoardSquare.h"

#include "inc\natives.h"
#include "inc\types.h"

#include "Utils.h"

ChessBoardSquare::ChessBoardSquare()
{
}


ChessBoardSquare::ChessBoardSquare(int file, int rank, bool isPromotion, bool isWhitePawnLine, bool isBlackPawnLine, Color color, Vector3 location, float headingWhite, float headingBlack, Vector3 delta1File, Vector3 delta1Rank)
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

	float actualZ = mLocation.z;
	if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(mLocation.x, mLocation.y, mLocation.z, &actualZ, 0)) {
		mLocation.z = actualZ + 0.1;
	}

	for (int i = 0; i < 4; i++) {
		Vector3 cornerLocation;
		float xModifier = -1.0f;
		float yModifier = -1.0f;

		if (i == 0 || i == 1) {
			xModifier = -1.0f;
		}
		else {
			xModifier = 1.0f;
		}
		if (i == 0 || i == 3) {
			yModifier = -1.0f;
		}
		else {
			yModifier = 1.0f;
		}


		cornerLocation.x = mLocation.x + (delta1File.x/2)  * xModifier + (delta1Rank.x/2)  * yModifier;
		cornerLocation.y = mLocation.y + (delta1File.y/2)  * xModifier + (delta1Rank.y/2)  * yModifier;
		//Initial approach of rotating all points didn't work as expected. Now just reusing delta values from ChessBoard
		//cornerLocation.x = cos(mHeadingWhite*PI / 180) * (cornerLocation.x - mLocation.x) - sin(mHeadingWhite*PI / 180) * (cornerLocation.y - mLocation.y) + mLocation.x;
		//cornerLocation.y = sin(mHeadingWhite*PI / 180) * (cornerLocation.x - mLocation.x) + cos(mHeadingWhite*PI / 180) * (cornerLocation.y - mLocation.y) + mLocation.y;

		cornerLocation.z = mLocation.z;
		float actualZ = mLocation.z;
		//calculate if possible
		if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(cornerLocation.x, cornerLocation.y, cornerLocation.z, &actualZ, 0)) {
			cornerLocation.z = actualZ + 0.02;
		}

		mCorners.push_back(cornerLocation);
	}

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

std::vector<Vector3> ChessBoardSquare::getCornerLocations()
{
	return mCorners;
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

Vector3 ChessBoardSquare::getScreenCoords()
{
	return mScreenCoords;
}

void ChessBoardSquare::updateScreenCoords()
{
	float xCoord = 0.0f;
	float yCoord = 0.0f;
	if (GRAPHICS::_WORLD3D_TO_SCREEN2D(mLocation.x, mLocation.y, mLocation.z, &xCoord, &yCoord)) {
		mScreenCoords.x = xCoord;
		mScreenCoords.y = yCoord;
	}
	else {
		mScreenCoords.x = 0.0f;
		mScreenCoords.y = 0.0f;
	}
}

void ChessBoardSquare::drawOnTick()
{
	/* Chessboard pattern https://trello.com/c/4LMI1319/92-draw-chess-board-somehow-graphicsdrawbox
	Vector3 baseLocation;
	baseLocation.x = 1629.0;
	baseLocation.y = 3215.0;
	baseLocation.z = 39.55;
	if (mColor == DARK) {
		GRAPHICS::DRAW_BOX(mLocation.x-1.0, mLocation.y-1.0, mLocation.z, mLocation.x + 1.0, mLocation.y + 1.0, mLocation.z, 10, 10, 10, 255);
	}
	else {
		GRAPHICS::DRAW_BOX(mLocation.x-1.0, mLocation.y-1.0, mLocation.z, mLocation.x + 1.0, mLocation.y + 1.0, mLocation.z, 255, 255, 255, 255);
	}
	*/

	int colorR = 0;
	int colorG = 0;
	int colorB = 0;
	//draw lines between corners. Last corner draws to first corner
	for (int i = 0; i < mCorners.size(); i++) {
		GRAPHICS::DRAW_LINE(mCorners[i].x, mCorners[i].y, mCorners[i].z, mCorners[(i+1)% mCorners.size()].x, mCorners[(i + 1) % mCorners.size()].y , mCorners[(i + 1) % mCorners.size()].z, colorR, colorG, colorB, 255);
	}


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



