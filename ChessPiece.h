#pragma once

#include "inc\types.h"
#include "ChessPed.h"
class ChessBoard;
#include "ChessBoard.h"

#include "ChessMove.h"
class ChessBattle;
#include "ChessBattle.h"
class ChessBattleFactory;
#include "ChessBattleFactory.h"
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
	ChessPiece(ChessSide::Side side, Type pieceType, ChessPed chessPed, Vector3 location, float heading, std::string primaryWeapon, std::string secondaryWeapon, std::string meleeWeapon);
	
	ChessSide::Side getSide();
	void setSide(ChessSide::Side side);
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
	void setHealth(int health);

	void startMovement(ChessMove chessMove, ChessBoard* chessBoard);
	bool isMovementCompleted(ChessMove chessMove, int nrChecksDone);

	std::shared_ptr<ChessBattle> startChessBattle(ChessMove chessMove,ChessBoard* chessBoard);

	std::string getPrimaryWeapon();
	void setPrimaryWeapon(std::string primaryWeapon);

	std::string getSecondaryWeapon();
	void setSecondaryWeapon(std::string secondaryWeapon);

	std::string getMeleeWeapon();
	void setMeleeWeapon(std::string meleeWeapon);

	void equipPrimaryWeapon();
	void equipSecondaryWeapon();
	void equipMeleeWeapon();
	void equipWeapon(std::string weaponName);
	void removeWeapons();

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
	std::string mPrimaryWeapon;
	std::string mSecondaryWeapon;
	std::string mMeleeWeapon;
	
};