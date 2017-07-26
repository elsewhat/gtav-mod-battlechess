#pragma once

#include "..\..\inc\types.h"
#include "ChessPed.h"
#include "ChessBoard.h"

#include "ChessMove.h"
#include <vector>

class ChessPiece {
public:
	enum Type {
		PAWN,
		KNIGHT,
		BISHOP,
		ROOK,
		QUEEN,
		KING
	};
	ChessPiece();
	ChessPiece(ChessSide::Side side, Type pieceType, ChessPed chessPed, Vector3 location, float heading);
	
	ChessSide::Side getSide();
	Type getPieceType() const;
	Vector3 getLocation() const;
	void setLocation(Vector3 location); 
	float getHeading() const;
	void setHeading(float heading);

	Ped getPed() const;
	ChessPed getChessPed() const;
	bool isPedDead() const;

	bool isPieceTaken() const;
	void setPieceTaken(bool pieceTaken);

	void spawnPed();
	void revivePed();
	void removePed();

	void startMovement(ChessMove chessMove);
	bool isMovementCompleted(ChessMove chessMove, int nrChecksDone);

	LPCSTR getPrimaryWeapon();
	void setPrimaryWeapon(LPCSTR primaryWeapon);

	LPCSTR getSecondaryWeapon();
	void setSecondaryWeapon(LPCSTR secondaryWeapon);

	LPCSTR getMeleeWeapon();
	void setMeleeWeapon(LPCSTR meleeWeapon);

	void equipPrimaryWeapon();
	void equipSecondaryWeapon();
	void equipMeleeWeapon();

protected:
	bool mPieceTaken = false;
	bool mIsMoving = false;
	bool mIsInBattle = false;
	float mWalkingSpeed = 1.0f;
	ChessPed mChessPed;
	ChessSide::Side mSide;
	Type mPieceType;
	Vector3 mLocation; 
	float mHeading; 
	LPCSTR mPrimaryWeapon;
	LPCSTR mSecondaryWeapon;
	LPCSTR mMeleeWeapon;
	
};