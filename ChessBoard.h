#pragma once


#include "..\..\inc\types.h"
#include "ChessPiece.h"
class ChessSet;
#include "ChessSet.h"

#include <vector>
#include <array>

//TODO: Do we really need a class for this ? leave it for now in order to be in line with CuteChess
class ChessSide {
public: 
	enum Side {
		WHITE,
		BLACK
	};

	static Side oppositeSide(Side side);

};

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


protected:
	int mSquareFile;
	int mSquareRank;
	Color mColor;
	ChessPiece* mPiece = nullptr;
	Vector3 mLocation;
	float mHeadingWhite;
	float mHeadingBlack;
	bool mIsPromotion;
};


class ChessBoard {
public:
	ChessBoard(Vector3 baseLocation, float squareDeltaX, float squareDeltaY);

	bool hasSideChessPieceAt(ChessSide::Side side, int rank, int file);
	ChessPiece* getChessPieceAt(int rank, int file);

	void setWhiteChessSet(ChessSet whiteSet);
	void setBlackChessSet(ChessSet blackSet);

protected:
	Vector3 mBaseLocation;
	float mSquareDeltaX;
	float mSquareDeltaY;
	std::array<ChessBoardSquare, 64>  mSquares;
	ChessSet mWhiteSet;
	ChessSet mBlackSet;

	void initializeSquares();
	ChessBoardSquare* getSquareAt(int rank, int file);
};
