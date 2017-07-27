#include "ChessBattle.h"

#include "inc\natives.h"
#include "Utils.h"
#include "GTAUtils.h"



ChessBattleFirePrimaryWeapon::ChessBattleFirePrimaryWeapon()
{
}

bool ChessBattleFirePrimaryWeapon::canBeExecutedFor(ChessMove chessMove)
{
	return true;
}


void ChessBattleFirePrimaryWeapon::startExecution(DWORD ticksStart, ChessMove chessMove)
{
	Logger::logDebug("ChessBattleFirePrimaryWeapon::startExecution");

	//Important to get from chessMove and not from ChessBoardSquare
	ChessPiece* attacker = chessMove.getAttacker();
	ChessPiece* defender = chessMove.getDefender();

	attacker->equipPrimaryWeapon();
	AI::TASK_SHOOT_AT_ENTITY(attacker->getPed() , defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_SINGLE_SHOT"));
}

bool ChessBattleFirePrimaryWeapon::isExecutionCompleted(DWORD ticksNow, ChessMove chessMove)
{
	//Important to get from chessMove and not from ChessBoardSquare
	ChessPiece* attacker = chessMove.getAttacker();
	ChessPiece* defender = chessMove.getDefender();


	if (defender->isPedDead()) {
		//TODO: Split into a standard method in abstract class
		Vector3 mLocation = chessMove.getSquareTo()->getLocation();
		float walkSpeed = 1.0;

		AI::TASK_GO_STRAIGHT_TO_COORD(attacker->getPed(), mLocation.x, mLocation.y, mLocation.z, attacker->getWalkSpeed(), -1, chessMove.getSquareTo()->getHeading(attacker->getSide()), 0.5f);
		//TODO: Wait for person to walk to square
		return true;
	}
	return false;
}

ChessBattle::ChessBattle()
{
}
