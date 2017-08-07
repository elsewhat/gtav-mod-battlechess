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
	mFiringPattern = "FIRING_PATTERN_SINGLE_SHOT";
}

ChessBattleFirePrimaryWeapon::ChessBattleFirePrimaryWeapon(std::string firingPattern)
{
	mFiringPattern = firingPattern;
}


void ChessBattleFirePrimaryWeapon::startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)
{
	Logger::logDebug("ChessBattleFirePrimaryWeapon::startExecution");
	mTicksStarted = ticksStart;
	//Important to get from chessMove and not from ChessBoardSquare
	ChessPiece* attacker = chessMove.getAttacker();
	ChessPiece* defender = chessMove.getDefender();

	//worst case is that two shots take 18 damage
	defender->setHealth(300);
	equipWeapon(chessMove);

	AI::TASK_SHOOT_AT_ENTITY(attacker->getPed() , defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY(strdup(mFiringPattern.c_str())));

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
		equipWeapon(chessMove);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(attacker->getPed());
		AI::TASK_SHOOT_AT_ENTITY(attacker->getPed(), defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY(strdup(mFiringPattern.c_str())));

		Logger::logDebug("ENTITY::DOES_ENTITY_EXIST(defender->getPed()) " + std::to_string(ENTITY::DOES_ENTITY_EXIST(defender->getPed())));

		//walkaround for issues with task_shoot_at_entity
		PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
		mTicksStarted = ticksNow;
	}
	return false;
}

void ChessBattleFirePrimaryWeapon::equipWeapon(ChessMove chessMove)
{
	chessMove.getAttacker()->equipPrimaryWeapon();
}

ChessBattleFireSecondaryWeapon::ChessBattleFireSecondaryWeapon():ChessBattleFirePrimaryWeapon()
{
}

ChessBattleFireSecondaryWeapon::ChessBattleFireSecondaryWeapon(std::string firingPattern):ChessBattleFirePrimaryWeapon(firingPattern)
{
}

void ChessBattleFireSecondaryWeapon::equipWeapon(ChessMove chessMove)
{
	chessMove.getAttacker()->equipSecondaryWeapon(); 
	chessMove.getDefender()->setHealth(115);
}

ChessBattleSyncedAnimation::ChessBattleSyncedAnimation(std::shared_ptr<SyncedAnimation> syncedAnimation, bool killAfterwards, bool useDefenderLocation,Vector3 locationOffset)
{
	mSyncedAnimation = syncedAnimation;
	mKillAfterwards = killAfterwards;
	mUseDefenderLocation = useDefenderLocation;
	mLocationOffset = locationOffset;
}

void ChessBattleSyncedAnimation::startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleSyncedAnimation::startExecution " + mSyncedAnimation->getTitle());
	std::vector<ChessPiece*> actors = { chessMove.getAttacker(), chessMove.getDefender() };

	Vector3 startLocation = ENTITY::GET_ENTITY_COORDS(chessMove.getAttacker()->getPed(), true);
	if (mUseDefenderLocation) {
		startLocation = ENTITY::GET_ENTITY_COORDS(chessMove.getDefender()->getPed(), true);
	}
	startLocation.x = startLocation.x+ mLocationOffset.x;
	startLocation.y = startLocation.y + mLocationOffset.y;
	startLocation.z = startLocation.z + mLocationOffset.z;
	mSyncedAnimation->executeSyncedAnimation(true, actors, false, startLocation, false, true, false);
}

bool ChessBattleSyncedAnimation::isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard * chessBoard)
{
	ChessPiece* defender = chessMove.getDefender();
	ChessPiece* attacker = chessMove.getAttacker();

	if (mIsMovingToSquare) {
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (mSyncedAnimation->isCompleted()) {
		mSyncedAnimation->cleanupAfterExecution(true, false);
		if (!defender->isPedDeadOrDying()) {
			Logger::logDebug("Defender is not dead. Exploding head death");
			if (mKillAfterwards) {
				PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
			}
			else {
				defender->removePed();
			}
		}
		mIsMovingToSquare = true;
		attacker->startMovement(chessMove, chessBoard);
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else {
		return false;
	}

}

ChessBattleHeadbutt::ChessBattleHeadbutt(ChessBoard* chessBoard)
{
	mSyncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("takedown_front_headbutt 2266");
}

void ChessBattleHeadbutt::startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleHeadbutt::startExecution " + mSyncedAnimation->getTitle());
	std::vector<ChessPiece*> actors = { chessMove.getAttacker(), chessMove.getDefender() };

	Vector3 startLocation = ENTITY::GET_ENTITY_COORDS(chessMove.getDefender()->getPed(), true);

	startLocation.x = startLocation.x + 0.0;
	startLocation.y = startLocation.y + 0.0;
	startLocation.z = startLocation.z + 0.0;
	mSyncedAnimation->executeSyncedAnimation(true, actors, false, startLocation, false, true, false);
}

bool ChessBattleHeadbutt::isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard * chessBoard)
{
	ChessPiece* defender = chessMove.getDefender();
	ChessPiece* attacker = chessMove.getAttacker();

	if (mIsMovingToSquare) {
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (mSyncedAnimation->isCompleted()) {
		mSyncedAnimation->cleanupAfterExecution(true, false);
		if (!defender->isPedDeadOrDying()) {
			defender->removePed();
		}
		mIsMovingToSquare = true;
		attacker->startMovement(chessMove, chessBoard);
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else {
		return false;
	}
}

ChessBattleSyncedAnimationChained::ChessBattleSyncedAnimationChained(std::shared_ptr<SyncedAnimation> firstSyncedAnimation, std::shared_ptr<SyncedAnimation> secondSyncedAnimation, bool killAfterwards, bool useDefenderLocation, Vector3 locationOffset)
{
	mFirstSyncedAnimation = firstSyncedAnimation;
	mSecondSyncedAnimation = secondSyncedAnimation;
	mKillAfterwards = killAfterwards;
	mUseDefenderLocation = useDefenderLocation;
	mLocationOffset = locationOffset;
}

void ChessBattleSyncedAnimationChained::startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleSyncedAnimation::startExecution " + mFirstSyncedAnimation->getTitle());
	std::vector<ChessPiece*> actors = { chessMove.getAttacker(), chessMove.getDefender() };

	mStartLocation = ENTITY::GET_ENTITY_COORDS(chessMove.getAttacker()->getPed(), true);
	if (mUseDefenderLocation) {
		mStartLocation = ENTITY::GET_ENTITY_COORDS(chessMove.getDefender()->getPed(), true);
	}
	mStartLocation.x = mStartLocation.x + mLocationOffset.x;
	mStartLocation.y = mStartLocation.y + mLocationOffset.y;
	mStartLocation.z = mStartLocation.z + mLocationOffset.z;
	mFirstSyncedAnimation->executeSyncedAnimation(true, actors, false, mStartLocation, false, true, false);
	mIsFirstCompleted = false;
}

bool ChessBattleSyncedAnimationChained::isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard * chessBoard)
{
	ChessPiece* defender = chessMove.getDefender();
	ChessPiece* attacker = chessMove.getAttacker();

	if (mIsMovingToSquare) {
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (mIsFirstCompleted && mSecondSyncedAnimation->isCompleted()) {
		Logger::logDebug("ChessBattleSyncedAnimationChained:: Second sync animation complete");
		mSecondSyncedAnimation->cleanupAfterExecution(true, false);
		if (!defender->isPedDeadOrDying()) {
			Logger::logDebug("Defender is not dead. Exploding head death");
			if (mKillAfterwards) {
				PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
			}
			else {
				defender->removePed();
			}
		}
		mIsMovingToSquare = true;
		attacker->startMovement(chessMove, chessBoard);
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (!mIsFirstCompleted && mFirstSyncedAnimation->isCompleted()) {
		Logger::logDebug("ChessBattleSyncedAnimationChained:: First sync animation complete");
		mFirstSyncedAnimation->cleanupAfterExecution(true, false);
		std::vector<ChessPiece*> actors = { chessMove.getAttacker(), chessMove.getDefender() };
		mSecondSyncedAnimation->executeSyncedAnimation(true, actors, false, mStartLocation, false, true, false);
		mIsFirstCompleted = true;
		return false;
	}
	else {
		return false;
	}
}

ChessBattleStealthKill::ChessBattleStealthKill()
{
}

void ChessBattleStealthKill::startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleStealthKill::startExecution");
	mTicksStarted = ticksStart;
	//Important to get from chessMove and not from ChessBoardSquare
	ChessPiece* attacker = chessMove.getAttacker();
	ChessPiece* defender = chessMove.getDefender();

	
	//AI::TASK_STEALTH_KILL(attacker->getPed(), defender->getPed(), GAMEPLAY::GET_HASH_KEY("AR_stealth_kill_knife"), 1.0f, 0);
	AI::TASK_STEALTH_KILL(attacker->getPed(), defender->getPed(), GAMEPLAY::GET_HASH_KEY("AR_stealth_kill_a"), 10.0f, 1);
}

bool ChessBattleStealthKill::isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard * chessBoard)
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
	}
	else if (ticksNow - mTicksStarted > 10000) {
		Logger::logDebug("ChessBattleStealthKill::More than 10000 ticks since started. Retriggering");

		//walkaround for issues with task_shoot_at_entity
		PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
		mTicksStarted = ticksNow;
	}
	return false;
}

ChessBattleDeathByCop::ChessBattleDeathByCop()
{
}

void ChessBattleDeathByCop::startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleStealthKill::startExecution");
	mTicksStarted = ticksStart;
	//Important to get from chessMove and not from ChessBoardSquare
	ChessPiece* attacker = chessMove.getAttacker();
	ChessPiece* defender = chessMove.getDefender();
	/*
	FireDepartment = 3,
	Paramedics = 5,
	Police = 7,
	PedsInCavalcades = 11, 
	Merryweather = 14
	*/
	mIncidentId = 0;
	GAMEPLAY::CREATE_INCIDENT_WITH_ENTITY(14, defender->getPed(),4, 0.0f, &mIncidentId);
}

bool ChessBattleDeathByCop::isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard * chessBoard)
{
	//Important to get from chessMove and not from ChessBoardSquare
	ChessPiece* attacker = chessMove.getAttacker();
	ChessPiece* defender = chessMove.getDefender();

	if (mIsMovingToSquare) {
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (defender->isPedDeadOrDying()) {
		mIsMovingToSquare = true;
		
		Logger::logDebug("GAMEPLAY::DELETE_INCIDENT");
		GAMEPLAY::DELETE_INCIDENT(&mIncidentId);

		attacker->startMovement(chessMove, chessBoard);



		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (ticksNow - mTicksStarted > 40000) {
		Logger::logDebug("ChessBattleStealthKill::More than 40000 ticks since started. Retriggering");

		//walkaround for issues with task_shoot_at_entity
		PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
		mTicksStarted = ticksNow;
	}
	return false;
}
