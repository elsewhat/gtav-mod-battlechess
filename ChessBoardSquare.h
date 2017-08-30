#pragma once

#include "inc\types.h"
class ChessPiece;
#include "ChessPiece.h"

#include <vector>

class ChessBoardSquare {
public:
	enum Color {
		LIGHT,
		DARK
	};
	ChessBoardSquare();
	ChessBoardSquare(int file, int rank, bool isPromotion, bool isWhitePawnLine, bool isBlackPawnLine, Color color, Vector3 location, float headingWhite, float headingBlack,Vector3 delta1File, Vector3 delta1Rank);

	int getSquareFile() const;
	int getSquareRank() const;
	Color getColor() const;

	bool isEmpty() const;
	ChessPiece* getPiece() const;
	void setPiece(ChessPiece* piece);
	void removePiece();
	Vector3 getLocation() const;
	void setLocation(Vector3 location);
	std::vector<Vector3> getCornerLocations();


	boolean isPromotion() const;

	boolean isPawnLine(ChessSide::Side side) const;

	boolean isEnpassentSquare(ChessSide::Side side) const;
	bool setEnpassentSquare(ChessSide::Side side);
	void removeEnpassentSquare();

	float getHeading(ChessSide::Side side) const;

	void ChessBoardSquare::drawSpotAbove(int colorR, int colorG, int colorB) const;

	bool doHighlightAsSelected() const;
	void setDoHighlightAsSelected(bool highlightAsSelected);

	bool doHighlightAsPossible() const;
	void setDoHighlightAsPossible(bool highlightAsPossible);

	bool doHighlightAsCursor() const;
	void setDoHighlightAsCursor(bool highlightAsCursor);

	bool equals(ChessBoardSquare* square) const;

	Vector3 getScreenCoords();
	void updateScreenCoords();

	void drawOnTick();
	std::string toString();
protected:
	bool hasChessPiece = false;
	int mSquareFile;
	int mSquareRank;
	Color mColor;
	ChessPiece* mPiece;
	Vector3 mLocation;
	Vector3 mScreenCoords;
	std::vector<Vector3> mCorners;
	float mHeadingWhite;
	float mHeadingBlack;
	bool mIsPromotion=false;
	bool mIsWhitePawnLine = false;
	bool mIsBlackPawnLine = false;

	bool mCanWhiteEnpassentCapture = false;
	bool mCanBlackEnpassentCapture = false;

	bool mHighlightAsSelected = false;
	bool mHighlightAsPossible = false;
	bool mHighlightAsCursor = false;
};