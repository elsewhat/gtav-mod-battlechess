#include "ChessBattleAction.h"
#include "inc\natives.h"

ChessBattleAction::ChessBattleAction()
{
}

bool ChessBattleAction::hasBeenStarted()
{
	return mHasBeenStarted;
}

ActionGoToCoord::ActionGoToCoord(Vector3 location, float heading, float walkingSpeed)
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
