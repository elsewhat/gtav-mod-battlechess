#include "ChessBattleAction.h"
#include "inc\natives.h"

ChessBattleAction::ChessBattleAction()
{
	mHasBeenStarted = false;
}

bool ChessBattleAction::hasBeenStarted()
{
	return mHasBeenStarted;
}

ActionGoToCoord::ActionGoToCoord(Vector3 location, float heading, float walkingSpeed):ChessBattleAction()
{
	mLocation = location;
	mHeading = heading;
	mWalkingSpeed = walkingSpeed;
	mMinDistance = 1.5;
}

void ActionGoToCoord::start(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	mHasBeenStarted = true;
	Logger::logDebug("ActionGoToCoord::start moving to (" + std::to_string(mLocation.x) + "," + std::to_string(mLocation.y) + "," + std::to_string(mLocation.z) + ")");
	AI::TASK_GO_STRAIGHT_TO_COORD(attacker->getPed(), mLocation.x, mLocation.y, mLocation.z, mWalkingSpeed, -1, mHeading, 0.5f);
}

bool ActionGoToCoord::checkForCompletion(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Vector3 currentLocation = ENTITY::GET_ENTITY_COORDS(attacker->getPed(), true);
	float distanceToTarget = SYSTEM::VDIST(mLocation.x, mLocation.y, mLocation.z, currentLocation.x, currentLocation.y, currentLocation.z);

	if (mIsCompleted) {
		return true;
	}else if(distanceToTarget <= mMinDistance) {
		mIsCompleted = true;
		return true;
	}
	else {
		return false;
	}


}

void ActionGoToCoord::setMinDistance(float minDistance)
{
	mMinDistance = minDistance;
}

ActionGoToSquare::ActionGoToSquare(ChessBoardSquare * squareTo, float heading, float walkingSpeed) : ActionGoToCoord(squareTo->getLocation(),heading, walkingSpeed)
{
	mMinDistance = 1.5;
}

ActionEnterVehicle::ActionEnterVehicle(Vehicle vehicle, float walkingSpeed, DWORD maxTicks) : ChessBattleAction()
{
	mVehicle = vehicle;
	mWalkingSpeed = walkingSpeed;
	mMaxTicks = maxTicks;
}

void ActionEnterVehicle::start(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	mHasBeenStarted = true;
	mTicksStarted = ticksStart;
	AI::TASK_ENTER_VEHICLE(attacker->getPed(), mVehicle, -1, -1, mWalkingSpeed, 1, 0);
	Logger::logDebug("ActionEnterVehicle::start");
}

bool ActionEnterVehicle::checkForCompletion(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ActionEnterVehicle::checkForCompletion");
	if (PED::IS_PED_SITTING_IN_VEHICLE(attacker->getPed(), mVehicle) || PED::IS_PED_IN_ANY_VEHICLE(attacker->getPed(), 0)) {
		mIsCompleted = true;
		return true;
	}
	else if (ticksNow - mTicksStarted > mMaxTicks) {
		Logger::logDebug("ActionEnterVehicle::checkForCompletion Teleporting to vehicle after " +std::to_string(ticksNow-mTicksStarted));
		teleportAttackerToVehicle(attacker->getPed());
		return false;
	}
	
	return false;
}

void ActionEnterVehicle::teleportAttackerToVehicle(Ped ped)
{
	PED::SET_PED_INTO_VEHICLE(ped, mVehicle, -1);
}

ActionThrowGrenade::ActionThrowGrenade(std::string weaponName, DWORD ticksBeforeComplete):ChessBattleAction()
{
	mWeaponName = weaponName;
	mTicksBeforeComplete = ticksBeforeComplete;
	mHasBeenStarted =false;
}

void ActionThrowGrenade::start(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	attacker->equipWeapon(mWeaponName);
	Vector3 targetLocation = defender->getLocation();
	AI::TASK_THROW_PROJECTILE(attacker->getPed(), targetLocation.x, targetLocation.y, targetLocation.z);

	mTicksStarted = ticksStart;
	mHasBeenStarted=true;
	mIsCompleted = false;
}

bool ActionThrowGrenade::checkForCompletion(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mIsCompleted) {
		return true;
	}
	else if (mHasBeenStarted && ticksNow - mTicksStarted > mTicksBeforeComplete) {
		mIsCompleted = true;
		return true;
	}
	else {
		return false;
	}
}
