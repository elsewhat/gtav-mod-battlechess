#pragma once

#include "inc\types.h"
#include "ChessPed.h"
class ChessBoard;
#include "ChessBoard.h"

#include "ChessMove.h"
class ChessBattle;
#include "ChessBattle.h"
#include <vector>
#include <memory>

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
	bool isPedDeadOrDying() const;

	bool isPieceTaken() const;
	void setPieceTaken(bool pieceTaken);

	void spawnPed(Hash relationshipGroupHash);
	void revivePed();
	void removePed();
	void setPedFreezed(bool isFreezed);
	void setPedCanBeDamaged(bool canBeDamaged);

	void startMovement(ChessMove chessMove, ChessBoard* chessBoard);
	bool isMovementCompleted(ChessMove chessMove, int nrChecksDone);

	std::shared_ptr<ChessBattle> startChessBattle(ChessMove chessMove,ChessBoard* chessBoard);

	LPCSTR getPrimaryWeapon();
	void setPrimaryWeapon(LPCSTR primaryWeapon);

	LPCSTR getSecondaryWeapon();
	void setSecondaryWeapon(LPCSTR secondaryWeapon);

	LPCSTR getMeleeWeapon();
	void setMeleeWeapon(LPCSTR meleeWeapon);

	void equipPrimaryWeapon();
	void equipSecondaryWeapon();
	void equipMeleeWeapon();
	void unequipWeapons();

	float getWalkSpeed();

protected:
	bool mPieceTaken = false;
	bool mIsMoving = false;
	bool mIsInBattle = false;
	float mWalkSpeed = 1.0f;
	ChessPed mChessPed;
	ChessSide::Side mSide;
	Type mPieceType;
	Vector3 mLocation; 
	float mHeading; 
	LPCSTR mPrimaryWeapon;
	LPCSTR mSecondaryWeapon;
	LPCSTR mMeleeWeapon;
	
};