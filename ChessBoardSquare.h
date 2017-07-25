#pragma once

#include "..\..\inc\types.h"
class ChessPiece;
#include "ChessPiece.h"

class ChessBoardSquare {
public:
	enum Color {
		LIGHT,
		DARK
	};
	ChessBoardSquare();
	ChessBoardSquare(int file, int rank, bool isPromotion, Color color, Vector3 location, float headingWhite, float headingBlack);

	int getSquareFile() const;
	int getSquareRank() const;
	Color getColor() const;

	bool isEmpty() const;
	ChessPiece* getPiece() const;
	void setPiece(ChessPiece* piece);
	void removePiece();
	Vector3 getLocation() const;
	void setLocation(Vector3 location);
	boolean isPromotion() const;

	float getHeadingWhite() const;
	float getHeadingBlack() const;

	void ChessBoardSquare::drawSpotAbove(int colorR, int colorG, int colorB) const;

	bool doHighlightAsSelected() const;
	void setDoHighlightAsSelected(bool highlightAsSelected);

	bool doHighlightAsPossible() const;
	void setDoHighlightAsPossible(bool highlightAsPossible);

	bool doHighlightAsCursor() const;
	void setDoHighlightAsCursor(bool highlightAsCursor);

	void drawOnTick();
protected:
	bool hasChessPiece = false;
	int mSquareFile;
	int mSquareRank;
	Color mColor;
	ChessPiece* mPiece;
	Vector3 mLocation;
	float mHeadingWhite;
	float mHeadingBlack;
	bool mIsPromotion;
	bool mHighlightAsSelected = false;
	bool mHighlightAsPossible = false;
	bool mHighlightAsCursor = false;
};