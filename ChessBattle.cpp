#include "ChessBattle.h"

#include "inc\natives.h"
#include "Utils.h"
#include "GTAUtils.h"

ChessBattle::ChessBattle()
{
}

void ChessBattle::initializeBattle(ChessMove chessMove, ChessBoard * chessBoard)
{
	std::vector<ChessPiece*> activePieces = { chessMove.getAttacker(), chessMove.getDefender() };
	chessBoard->freezeAllExcept(activePieces);
	chessMove.getAttacker()->setPedCanBeDamaged(false);
	chessMove.getDefender()->setPedCanBeDamaged(true);
	mIsMovingToSquare = false;
	mNrMovementChecks = 0;
}

ChessBattleFirePrimaryWeapon::ChessBattleFirePrimaryWeapon()
{
}


void ChessBattleFirePrimaryWeapon::startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)
{
	Logger::logDebug("ChessBattleFirePrimaryWeapon::startExecution");
	mTicksStarted = ticksStart;
	//Important to get from chessMove and not from ChessBoardSquare
	ChessPiece* attacker = chessMove.getAttacker();
	ChessPiece* defender = chessMove.getDefender();

	//worst case is that two shots take 18 damage
	defender->setHealth(115);
	attacker->equipPrimaryWeapon();
	AI::TASK_SHOOT_AT_ENTITY(attacker->getPed() , defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_SINGLE_SHOT"));

	Logger::logDebug("startExecution Defender health " + std::to_string(ENTITY::GET_ENTITY_HEALTH(defender->getPed())));
}

bool ChessBattleFirePrimaryWeapon::isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)
{
	//Important to get from chessMove and not from ChessBoardSquare
	ChessPiece* attacker = chessMove.getAttacker();
	ChessPiece* defender = chessMove.getDefender();

	if (mIsMovingToSquare) {
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (defender->isPedDeadOrDying()) {
		mIsMovingToSquare = true; 
		attacker->startMovement(chessMove, chessBoard);

		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
		/*
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
		return true;*/
	}
	else if (ticksNow - mTicksStarted > 5000) {
		Logger::logDebug("ChessBattleFirePrimaryWeapon::More than 5000 ticks since started. Retriggering");

		Logger::logDebug("Defender health " + std::to_string(ENTITY::GET_ENTITY_HEALTH(defender->getPed())));
		attacker->removeWeapons();
		attacker->equipPrimaryWeapon();
		AI::CLEAR_PED_TASKS_IMMEDIATELY(attacker->getPed());
		AI::TASK_SHOOT_AT_ENTITY(attacker->getPed(), defender->getPed(), 5000, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_SINGLE_SHOT"));

		Logger::logDebug("ENTITY::DOES_ENTITY_EXIST(defender->getPed()) " + std::to_string(ENTITY::DOES_ENTITY_EXIST(defender->getPed())));

		//walkaround for issues with task_shoot_at_entity
		PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
		mTicksStarted = ticksNow;
	}
	return false;
}


