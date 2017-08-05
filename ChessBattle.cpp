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
	mTicksStarted = ticksStart;
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


	if (defender->isPedDeadOrDying()) {
		//TODO: Split into a standard method in abstract class
		Vector3 mLocation = chessMove.getSquareTo()->getLocation();
		float walkSpeed = 1.0;

		//attacker->unequipWeapons();

		TaskSequence task_seq = 1;
		AI::OPEN_SEQUENCE_TASK(&task_seq);

		AI::TASK_RELOAD_WEAPON(0, true);
		AI::TASK_GO_STRAIGHT_TO_COORD(0, mLocation.x, mLocation.y, mLocation.z, attacker->getWalkSpeed(), -1, chessMove.getSquareTo()->getHeading(attacker->getSide()), 0.5f);

		AI::CLOSE_SEQUENCE_TASK(task_seq);
		AI::TASK_PERFORM_SEQUENCE(attacker->getPed(), task_seq);
		AI::CLEAR_SEQUENCE_TASK(&task_seq);

		//TODO: Wait for person to walk to square
		return true;
	}
	else if (ticksNow - mTicksStarted > 5000) {
		Logger::logDebug("ChessBattleFirePrimaryWeapon::More than 5000 ticks since started. Retriggering");
		AI::CLEAR_PED_TASKS(attacker->getPed());
		AI::TASK_SHOOT_AT_ENTITY(attacker->getPed(), defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_SINGLE_SHOT"));
		mTicksStarted = ticksNow;
	}
	return false;
}

ChessBattle::ChessBattle()
{
}
