#include "ChessBattle.h"

#include "..\..\inc\natives.h"
#include "Utils.h"
#include "GTAUtils.h"

ChessBattleFirePrimaryWeapon::ChessBattleFirePrimaryWeapon()
{
}

bool ChessBattleFirePrimaryWeapon::canBeExecutedFor(const ChessPiece attacker, const ChessPiece defender)
{
	return true;
}


void ChessBattleFirePrimaryWeapon::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, const ChessBoardSquare squareFrom, const ChessBoardSquare squareTo)
{
	attacker->equipPrimaryWeapon();
	AI::TASK_SHOOT_AT_ENTITY(attacker->getPed() , defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_SINGLE_SHOT"));
}

bool ChessBattleFirePrimaryWeapon::isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, const ChessBoardSquare squareFrom, const ChessBoardSquare squareTo)
{
	if (defender->isPedDead()) {
		//TODO: Split into a standard method in abstract class
		Vector3 mLocation = squareTo.getLocation();
		float walkSpeed = 1.0;
		float heading = squareTo.getHeadingWhite();
		if (attacker->getSide() == ChessSide::WHITE) {
			heading = squareTo.getHeadingBlack();
		}

		AI::TASK_GO_STRAIGHT_TO_COORD(attacker->getPed(), mLocation.x, mLocation.y, mLocation.z, walkSpeed, -1, heading, 0.5f);
		//TODO: Wait for person to walk to square
		return true;
	}


	return false;
}
