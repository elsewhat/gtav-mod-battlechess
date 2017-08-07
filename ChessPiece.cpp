#include "ChessPiece.h"

#include "inc\natives.h"
#include "Utils.h"
#include "GTAUtils.h"

ChessPiece::ChessPiece()
{
}

ChessPiece::ChessPiece(ChessSide::Side side, Type pieceType, ChessPed chessPed, Vector3 location, float heading)
{
	mSide = side;
	mPieceType = pieceType;
	mChessPed = chessPed;
	mLocation = location;
	mHeading = heading;
	mPrimaryWeapon = "WEAPON_COMBATPISTOL";
}

ChessSide::Side ChessPiece::getSide()
{
	return mSide;
}

void ChessPiece::setSide(ChessSide::Side side)
{
	mSide = side;
}

ChessPiece::Type ChessPiece::getPieceType() const
{
	return mPieceType;
}

Vector3 ChessPiece::getLocation() const
{
	return mLocation;
}

void ChessPiece::setLocation(Vector3 location)
{
	mLocation = location;
	
	/*if (mChessPed.getPed() > 0) {
		Logger::logDebug("Teleporting piece of type " + std::to_string(mPieceType) + " to new location");
		GTAModUtils::teleportEntityToLocation(mChessPed.getPed(), location, false);
	}
	else {
		Logger::logError("ChessPiece::setLocation mPed is not >0 but " + std::to_string(mChessPed.getPed()));
	}*/
}

float ChessPiece::getHeading() const
{
	return mHeading;
}

void ChessPiece::setHeading(float heading)
{
	mHeading = heading;
}

Ped ChessPiece::getPed() const
{
	return mChessPed.getPed();
}

ChessPed ChessPiece::getChessPed() const
{
	return  mChessPed;
}


bool ChessPiece::isPedDeadOrDying() const
{
	return mChessPed.isPedDeadOrDying();
}

bool ChessPiece::isPieceTaken() const
{
	return mPieceTaken;
}

void ChessPiece::setPieceTaken(bool pieceTaken)
{
	mPieceTaken = pieceTaken;
}

void ChessPiece::spawnPed(Hash relationshipGroupHash)
{
	mChessPed.spawnPed(mLocation, mHeading, relationshipGroupHash);
	if (mPieceType == ChessPiece::KING) {
		PED::GIVE_PED_HELMET(getPed(), false, 4096, -1);
	}
}

void ChessPiece::revivePed()
{
	mChessPed.revivePed(mLocation, mHeading);
}

void ChessPiece::removePed()
{
	mChessPed.removePed();
}

void ChessPiece::setPedFreezed(bool isFreezed)
{
	ENTITY::FREEZE_ENTITY_POSITION(getPed(), isFreezed);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(getPed(), isFreezed);
}

void ChessPiece::setPedCanBeDamaged(bool canBeDamaged)
{
	ENTITY::SET_ENTITY_CAN_BE_DAMAGED(getPed(), canBeDamaged);
}

void ChessPiece::setHealth(int health)
{
	ENTITY::SET_ENTITY_HEALTH(getPed(), health);
}

void ChessPiece::startMovement(ChessMove chessMove, ChessBoard* chessBoard)
{
	mIsMoving = true;
	chessMove.getAttacker()->setPedFreezed(false);

	Vector3 squareToLocation = mLocation;
	if (mPieceType == KNIGHT) {
		//Rook first move up in rank-direction, then sideways in file-direction
		TaskSequence task_seq = 1;
		AI::OPEN_SEQUENCE_TASK(&task_seq);

		ChessBoardSquare* squareFrom = chessMove.getSquareFrom();
		ChessBoardSquare* squareTo = chessMove.getSquareTo();
		ChessBoardSquare* squareIntermediate = chessBoard->getSquareAt(squareTo->getSquareRank(), squareFrom->getSquareFile());

		AI::TASK_GO_STRAIGHT_TO_COORD(0, squareIntermediate->getLocation().x, squareIntermediate->getLocation().y, squareIntermediate->getLocation().z, mWalkSpeed, -1, squareIntermediate->getHeading(mSide), 0.5f);
		AI::TASK_GO_STRAIGHT_TO_COORD(0, squareToLocation.x, squareToLocation.y, squareToLocation.z, mWalkSpeed, -1, chessMove.getSquareTo()->getHeading(mSide), 0.5f);

		AI::CLOSE_SEQUENCE_TASK(task_seq);
		AI::TASK_PERFORM_SEQUENCE(getPed(), task_seq);
		AI::CLEAR_SEQUENCE_TASK(&task_seq);
	}
	else {
		AI::TASK_GO_STRAIGHT_TO_COORD(getPed(), squareToLocation.x, squareToLocation.y, squareToLocation.z, mWalkSpeed, -1, chessMove.getSquareTo()->getHeading(mSide), 0.5f);
	}
}


bool ChessPiece::isMovementCompleted(ChessMove chessMove, int nrChecksDone)
{
	float minDistance = 0.5f;
	Vector3 currentLocation  = ENTITY::GET_ENTITY_COORDS(getPed(), true);
	float distanceToTarget = SYSTEM::VDIST(mLocation.x, mLocation.y, mLocation.z, currentLocation.x, currentLocation.y, currentLocation.z);
	if (distanceToTarget < minDistance) {
		return true;
	}
	else {
		return false;
	}
}

std::shared_ptr<ChessBattle> ChessPiece::startChessBattle(ChessMove chessMove, ChessBoard* chessBoard)
{
	std::shared_ptr<ChessBattle> chessBattle = chessBoard->getChessBattleFactory()->findBattle(chessMove, chessBoard);
	chessBattle->initializeBattle(chessMove, chessBoard);
	chessBattle->startExecution(GetTickCount(), chessMove);

	return chessBattle;
}

LPCSTR ChessPiece::getPrimaryWeapon()
{
	return mPrimaryWeapon;
}

void ChessPiece::setPrimaryWeapon(LPCSTR primaryWeapon)
{
	mPrimaryWeapon = primaryWeapon;
}

LPCSTR ChessPiece::getSecondaryWeapon()
{
	return mSecondaryWeapon;
}

void ChessPiece::setSecondaryWeapon(LPCSTR secondaryWeapon)
{
	mSecondaryWeapon = secondaryWeapon;
}

LPCSTR ChessPiece::getMeleeWeapon()
{
	return mMeleeWeapon;
}

void ChessPiece::setMeleeWeapon(LPCSTR meleeWeapon)
{
	mMeleeWeapon = meleeWeapon;
}

void ChessPiece::equipPrimaryWeapon()
{
	if (mPrimaryWeapon != NULL || mPrimaryWeapon[0] != 0) {
		WeaponUtils::giveWeapon(mChessPed.getPed(), mPrimaryWeapon);
		//WEAPON::SET_PED_INFINITE_AMMO_CLIP(mChessPed.getPed(), true);
	}
}

void ChessPiece::equipSecondaryWeapon()
{
	if (mSecondaryWeapon == NULL || mSecondaryWeapon[0] == 0) {
		WeaponUtils::giveWeapon(mChessPed.getPed(), mSecondaryWeapon);
	}
}

void ChessPiece::equipMeleeWeapon()
{
	if (mSecondaryWeapon == NULL || mSecondaryWeapon[0] == 0) {
		WeaponUtils::giveWeapon(mChessPed.getPed(), mMeleeWeapon);
	}
}

void ChessPiece::removeWeapons()
{
	WEAPON::REMOVE_ALL_PED_WEAPONS(getPed(), true);
}

float ChessPiece::getWalkSpeed()
{
	return mWalkSpeed;
}

