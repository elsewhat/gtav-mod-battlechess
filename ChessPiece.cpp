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
	mPrimaryWeapon = "WEAPON_PUMPSHOTGUN";
	mSecondaryWeapon = "WEAPON_COMBATPISTOL";
	mMeleeWeapon = "WEAPON_KNIFE";
}

ChessPiece::ChessPiece(ChessSide::Side side, Type pieceType, ChessPed chessPed, Vector3 location, float heading, std::string primaryWeapon, std::string secondaryWeapon, std::string meleeWeapon)
{
	mSide = side;
	mPieceType = pieceType;
	mChessPed = chessPed;
	mLocation = location;
	mHeading = heading;
	mPrimaryWeapon = primaryWeapon;
	mSecondaryWeapon = secondaryWeapon;
	mMeleeWeapon = meleeWeapon;
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

std::string ChessPiece::getPieceTypeString() const
{
	switch (mPieceType)
	{
	case ChessPiece::PAWN:
		return "Pawn";
		break;
	case ChessPiece::KNIGHT:
		return "Knight";
		break;
	case ChessPiece::BISHOP:
		return "Bishop";
		break;
	case ChessPiece::ROOK:
		return "Rook";
		break;
	case ChessPiece::QUEEN:
		return "Queen";
		break;
	case ChessPiece::KING:
		return "King";
		break;
	default:
		break;
	}
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

void ChessPiece::setPed(Ped ped)
{
	mChessPed.setPed(ped);
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

void ChessPiece::forceFaceHeadingTowards(ChessPiece * pieceToFace)
{
	Vector3 myLocation = ENTITY::GET_ENTITY_COORDS(getPed(),1);
	Vector3 otherLocation = ENTITY::GET_ENTITY_COORDS(pieceToFace->getPed(), 1);
	float heading = GAMEPLAY::GET_HEADING_FROM_VECTOR_2D(otherLocation.x - myLocation.x, otherLocation.y - myLocation.y);
	ENTITY::SET_ENTITY_HEADING(getPed(), heading);
}

void ChessPiece::spawnPed(Hash relationshipGroupHash)
{
	mChessPed.spawnPed(mLocation, mHeading, relationshipGroupHash);
	if (mPieceType == ChessPiece::KING) {
		PED::GIVE_PED_HELMET(getPed(), false, 4096, -1);
	}
	setPedCanBeDamaged(false);
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
	//http://gtaforums.com/topic/852786-q-stop-freeroam-peds-from-fleeing/
	//http://gtaforums.com/topic/793401-stop-peds-from-fleeing-method-2/
	PED::SET_PED_COMBAT_ATTRIBUTES(getPed(), 17, isFreezed);
	//PED::SET_PED_FLEE_ATTRIBUTES(getPed(), 0, !isFreezed);
	if (isFreezed) {
		PED::SET_PED_SEEING_RANGE(getPed(), 0.0f);
		PED::SET_PED_HEARING_RANGE(getPed(), 0.0f);
		PED::SET_PED_ALERTNESS(getPed(), 0);
	}
	else {
		PED::SET_PED_SEEING_RANGE(getPed(), 50.f);
		PED::SET_PED_HEARING_RANGE(getPed(), 20.0f);
		PED::SET_PED_ALERTNESS(getPed(), 3);
	}

}

void ChessPiece::setPedCanBeDamaged(bool canBeDamaged)
{
	ENTITY::SET_ENTITY_CAN_BE_DAMAGED(getPed(), canBeDamaged);
	ENTITY::SET_ENTITY_INVINCIBLE(getPed(), !canBeDamaged);
	//ENTITY::SET_ENTITY_PROOFS(getPed(), !canBeDamaged, !canBeDamaged, !canBeDamaged, !canBeDamaged, !canBeDamaged, !canBeDamaged, !canBeDamaged, !canBeDamaged);
}

void ChessPiece::setHealth(int health)
{
	ENTITY::SET_ENTITY_HEALTH(getPed(), health);
}

void ChessPiece::startMovement(ChessMove chessMove, ChessBoard* chessBoard, bool ignorePieceType)
{
	Logger::logDebug("ChessPiece::startMovement");
	mIsMoving = true;
	chessMove.getAttacker()->setPedFreezed(false);

	Vector3 squareToLocation = mLocation;
	if (mPieceType == KNIGHT && !ignorePieceType) {
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
	float minDistance = 1.5f;
	Vector3 currentLocation  = ENTITY::GET_ENTITY_COORDS(getPed(), true);
	float distanceToTarget = SYSTEM::VDIST(mLocation.x, mLocation.y, mLocation.z, currentLocation.x, currentLocation.y, currentLocation.z);
	Logger::logDebug("ChessPiece::isMovementCompleted distance:" + std::to_string(distanceToTarget));
	if (distanceToTarget < minDistance) {
		Logger::logDebug("ChessPiece::isMovementCompleted distance:" + std::to_string(distanceToTarget));
		return true;
	}
	else {
		return false;
	}
}

std::shared_ptr<ChessBattle> ChessPiece::startChessBattle(ChessMove chessMove, ChessBoard* chessBoard)
{
	std::shared_ptr<ChessBattle> chessBattle = chessBoard->getChessBattleFactory()->findBattle(chessMove, chessBoard);
	chessBattle->initializeBattle(chessMove.getAttacker(), chessMove.getDefender(), chessMove, chessBoard);
	chessBattle->startExecution(GetTickCount(), chessMove.getAttacker(), chessMove.getDefender(), chessMove, chessBoard);

	return chessBattle;
}

std::string ChessPiece::getPrimaryWeapon()
{
	return mPrimaryWeapon;
}

void ChessPiece::setPrimaryWeapon(std::string primaryWeapon)
{
	mPrimaryWeapon = primaryWeapon;
}

std::string ChessPiece::getSecondaryWeapon()
{
	return mSecondaryWeapon;
}

void ChessPiece::setSecondaryWeapon(std::string secondaryWeapon)
{
	mSecondaryWeapon = secondaryWeapon;
}

std::string ChessPiece::getMeleeWeapon()
{
	return mMeleeWeapon;
}

void ChessPiece::setMeleeWeapon(std::string meleeWeapon)
{
	mMeleeWeapon = meleeWeapon;
}

void ChessPiece::equipPrimaryWeapon()
{
	WeaponUtils::giveWeapon(mChessPed.getPed(), mPrimaryWeapon);
}

void ChessPiece::equipSecondaryWeapon()
{
	WeaponUtils::giveWeapon(mChessPed.getPed(), mSecondaryWeapon);
}

void ChessPiece::equipMeleeWeapon()
{
	WeaponUtils::giveWeapon(mChessPed.getPed(), mMeleeWeapon);
}

void ChessPiece::equipWeapon(std::string weaponName)
{
	WeaponUtils::giveWeapon(mChessPed.getPed(), weaponName);
}

void ChessPiece::removeWeapons()
{
	WEAPON::REMOVE_ALL_PED_WEAPONS(getPed(), true);
}

float ChessPiece::getWalkSpeed()
{
	return mWalkSpeed;
}

