#include "ChessPiece.h"

#include "..\..\inc\natives.h"
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
}

ChessSide::Side ChessPiece::getSide()
{
	return mSide;
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


bool ChessPiece::isPedDead() const
{
	mChessPed.isPedDead();
}

bool ChessPiece::isPieceTaken() const
{
	return mPieceTaken;
}

void ChessPiece::setPieceTaken(bool pieceTaken)
{
	mPieceTaken = pieceTaken;
}

void ChessPiece::spawnPed()
{
	mChessPed.spawnPed(mLocation, mHeading);
}

void ChessPiece::revivePed()
{
	mChessPed.revivePed(mLocation, mHeading);
}

void ChessPiece::removePed()
{
	mChessPed.removePed();
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
	if (mPrimaryWeapon == NULL || mPrimaryWeapon[0] == 0) {
		WeaponUtils::giveWeapon(mChessPed.getPed(), mPrimaryWeapon);
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

