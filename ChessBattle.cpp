#include "ChessBattle.h"

#include "inc\natives.h"
#include "Utils.h"
#include "GTAUtils.h"

ChessBattle::ChessBattle(ChessMove chessMove, ChessBoard* chessBoard)
{
	ChessBoardSquare* squareTo = chessMove.getSquareTo();
	mActionGoToEndSquare = std::make_shared<ActionGoToSquare>(ActionGoToSquare(squareTo, squareTo->getHeading(chessMove.getAttacker()->getSide()), 1.0));
}

void ChessBattle::initializeBattle(ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	std::vector<ChessPiece*> activePieces = { attacker, defender };
	chessBoard->freezeAllExcept(activePieces);
	attacker->setPedCanBeDamaged(false);
	defender->setPedCanBeDamaged(true);
	mIsMovingToSquare = false;
	mNrMovementChecks = 0;
}

ChessBattleFirePrimaryWeapon::ChessBattleFirePrimaryWeapon(ChessMove chessMove, ChessBoard* chessBoard) : ChessBattle(chessMove, chessBoard)
{
	mFiringPattern = "FIRING_PATTERN_SINGLE_SHOT";
}

ChessBattleFirePrimaryWeapon::ChessBattleFirePrimaryWeapon(ChessMove chessMove, ChessBoard* chessBoard, std::string firingPattern) : ChessBattle(chessMove, chessBoard)
{
	mFiringPattern = firingPattern;
}


void ChessBattleFirePrimaryWeapon::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)
{
	Logger::logDebug("ChessBattleFirePrimaryWeapon::startExecution");
	mTicksStarted = ticksStart;

	//worst case is that two shots take 18 damage
	defender->setHealth(300);
	equipWeapon(chessMove);

	AI::TASK_SHOOT_AT_ENTITY(attacker->getPed() , defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY(strdup(mFiringPattern.c_str())));

	Logger::logDebug("startExecution Defender health " + std::to_string(ENTITY::GET_ENTITY_HEALTH(defender->getPed())));
}

bool ChessBattleFirePrimaryWeapon::isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)
{
	if (mIsMovingToSquare) {
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (defender->isPedDeadOrDying()) {
		mIsMovingToSquare = true; 
		attacker->startMovement(chessMove, chessBoard, true);

		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
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

ChessBattleFireSecondaryWeapon::ChessBattleFireSecondaryWeapon(ChessMove chessMove, ChessBoard* chessBoard) :ChessBattleFirePrimaryWeapon(chessMove, chessBoard)
{
}

ChessBattleFireSecondaryWeapon::ChessBattleFireSecondaryWeapon(ChessMove chessMove, ChessBoard* chessBoard, std::string firingPattern) : ChessBattleFirePrimaryWeapon(chessMove, chessBoard, firingPattern)
{
}

void ChessBattleFireSecondaryWeapon::equipWeapon(ChessMove chessMove)
{
	chessMove.getAttacker()->equipSecondaryWeapon(); 
	chessMove.getDefender()->setHealth(115);
}

ChessBattleSyncedAnimation::ChessBattleSyncedAnimation(ChessMove chessMove, ChessBoard* chessBoard, std::shared_ptr<SyncedAnimation> syncedAnimation, bool killAfterwards, bool useDefenderLocation,Vector3 locationOffset) : ChessBattle(chessMove, chessBoard)
{
	mSyncedAnimation = syncedAnimation;
	mKillAfterwards = killAfterwards;
	mUseDefenderLocation = useDefenderLocation;
	mLocationOffset = locationOffset;
}

void ChessBattleSyncedAnimation::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleSyncedAnimation::startExecution " + mSyncedAnimation->getTitle());
	std::vector<ChessPiece*> actors = { attacker, defender };

	Vector3 startLocation = ENTITY::GET_ENTITY_COORDS(attacker->getPed(), true);
	if (mUseDefenderLocation) {
		startLocation = ENTITY::GET_ENTITY_COORDS(defender->getPed(), true);
	}
	startLocation.x = startLocation.x+ mLocationOffset.x;
	startLocation.y = startLocation.y + mLocationOffset.y;
	startLocation.z = startLocation.z + mLocationOffset.z;
	mSyncedAnimation->executeSyncedAnimation(true, actors, false, startLocation, false, true, false);
}

bool ChessBattleSyncedAnimation::isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
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
		attacker->startMovement(chessMove, chessBoard,true);
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else {
		return false;
	}

}

ChessBattleManualPlacedSyncedAnimation::ChessBattleManualPlacedSyncedAnimation(ChessMove chessMove, ChessBoard * chessBoard) : ChessBattle(chessMove, chessBoard)
{
	//default to be overriddden by subclasses
	mSyncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("takedown_front_headbutt 2266");
	mSpeedBeforeImpact = 2.0;
	mForceHeading = false;
	mTicksBeforeAction = 700;
}

ChessBattleHeadbutt::ChessBattleHeadbutt(ChessMove chessMove, ChessBoard * chessBoard) : ChessBattleManualPlacedSyncedAnimation(chessMove, chessBoard)
{
	mSyncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("takedown_front_headbutt 2266");
	mSpeedBeforeImpact = 2.0;
	mForceHeading = false; 
	mTicksBeforeAction = 700;
}

void ChessBattleManualPlacedSyncedAnimation::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleManualPlacedSyncedAnimation::startExecution " + mSyncedAnimation->getTitle());
	
	//1. Move to square ahead of defender
	//2. Start running to square of defender
	//3. After a few hundred ms trigger synched anim
	//4. Wait for synched anim to complete
	//5. Move to target square
	//6. Wait for getting close to target square

	mIsMovingIntoPosition = true;
	mSquareSetup = chessBoard->getSquareInFrontOf(chessMove.getSquareTo(), attacker->getSide());
	Vector3 squareToLocation = mSquareSetup->getLocation();
	AI::TASK_GO_STRAIGHT_TO_COORD(attacker->getPed(), squareToLocation.x, squareToLocation.y, squareToLocation.z, 1.0, -1, mSquareSetup->getHeading(attacker->getSide()), 0.5f);
}

bool ChessBattleManualPlacedSyncedAnimation::isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mActionGoToEndSquare->hasBeenStarted()) {
		return mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard);
	}
	else if (mIsMovingIntoPosition) {

		Vector3 currentLocation = ENTITY::GET_ENTITY_COORDS(attacker->getPed(), true);
		Vector3 squareToLocation = mSquareSetup->getLocation();
		float distanceToTarget = SYSTEM::VDIST(squareToLocation.x, squareToLocation.y, squareToLocation.z, currentLocation.x, currentLocation.y, currentLocation.z);

		if (distanceToTarget < 1.5) {
			mIsMovingIntoPosition = false;
			Vector3 squareToLocation = chessMove.getSquareTo()->getLocation();
			AI::TASK_GO_STRAIGHT_TO_COORD(attacker->getPed(), squareToLocation.x, squareToLocation.y, squareToLocation.z, mSpeedBeforeImpact, -1, mSquareSetup->getHeading(attacker->getSide()), 0.5f);
			mIsWaitingForTriggeringSynced = true;
			mTicksStarted = ticksNow;
		}
		return false;
	}
	else if (mIsWaitingForTriggeringSynced) {
		if (ticksNow - mTicksStarted > mTicksBeforeAction) {
			mIsWaitingForTriggeringSynced = false;

			std::vector<ChessPiece*> actors = { attacker, defender };

			Vector3 startLocation = ENTITY::GET_ENTITY_COORDS(defender->getPed(), true);

			startLocation.x = startLocation.x + 0.0;
			startLocation.y = startLocation.y + 0.0;
			startLocation.z = startLocation.z + 0.0;
			if (mForceHeading) {
				ENTITY::SET_ENTITY_HEADING(attacker->getPed(), mSquareSetup->getHeading(attacker->getSide()));
			}
			mSyncedAnimation->executeSyncedAnimation(true, actors, false, startLocation, false, true, false);
		}
		return false;
	}
	else if (mSyncedAnimation->isCompleted()) {
		mSyncedAnimation->cleanupAfterExecution(true, false);
		if (!defender->isPedDeadOrDying()) {
			defender->removePed();
		}
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
		return false;
	}
	return false;
}

ChessBattleSyncedAnimationChained::ChessBattleSyncedAnimationChained(ChessMove chessMove, ChessBoard * chessBoard,std::shared_ptr<SyncedAnimation> firstSyncedAnimation, std::shared_ptr<SyncedAnimation> secondSyncedAnimation, bool killAfterwards, bool useDefenderLocation, Vector3 locationOffset, bool hasDefenderWeapon, std::string defenderWeapon) : ChessBattle(chessMove, chessBoard)
{
	mFirstSyncedAnimation = firstSyncedAnimation;
	mSecondSyncedAnimation = secondSyncedAnimation;
	mKillAfterwards = killAfterwards;
	mUseDefenderLocation = useDefenderLocation;
	mLocationOffset = locationOffset;
	mHasDefenderWeapon = hasDefenderWeapon;
	mDefenderWeapon = defenderWeapon;
}

void ChessBattleSyncedAnimationChained::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleSyncedAnimation::startExecution " + mFirstSyncedAnimation->getTitle());
	
	if (mHasDefenderWeapon) {
		defender->equipWeapon(mDefenderWeapon);
	}
	
	std::vector<ChessPiece*> actors = { attacker, defender };

	mStartLocation = ENTITY::GET_ENTITY_COORDS(attacker->getPed(), true);
	if (mUseDefenderLocation) {
		mStartLocation = ENTITY::GET_ENTITY_COORDS(defender->getPed(), true);
	}
	mStartLocation.x = mStartLocation.x + mLocationOffset.x;
	mStartLocation.y = mStartLocation.y + mLocationOffset.y;
	mStartLocation.z = mStartLocation.z + mLocationOffset.z;
	mFirstSyncedAnimation->executeSyncedAnimation(true, actors, false, mStartLocation, false, true, false);
	mIsFirstCompleted = false;
}

bool ChessBattleSyncedAnimationChained::isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
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
		attacker->startMovement(chessMove, chessBoard,true);
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

ChessBattleStealthKill::ChessBattleStealthKill(ChessMove chessMove, ChessBoard * chessBoard) : ChessBattle(chessMove, chessBoard)
{
}

void ChessBattleStealthKill::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleStealthKill::startExecution");
	mTicksStarted = ticksStart;
	
	//AI::TASK_STEALTH_KILL(attacker->getPed(), defender->getPed(), GAMEPLAY::GET_HASH_KEY("AR_stealth_kill_knife"), 1.0f, 0);
	AI::TASK_STEALTH_KILL(attacker->getPed(), defender->getPed(), GAMEPLAY::GET_HASH_KEY("AR_stealth_kill_a"), 10.0f, 1);
}

bool ChessBattleStealthKill::isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mIsMovingToSquare) {
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (defender->isPedDeadOrDying()) {
		mIsMovingToSquare = true;
		attacker->startMovement(chessMove, chessBoard,true);

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

ChessBattleDeathByCop::ChessBattleDeathByCop(ChessMove chessMove, ChessBoard * chessBoard) : ChessBattle(chessMove, chessBoard)
{
}

void ChessBattleDeathByCop::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleStealthKill::startExecution");
	mTicksStarted = ticksStart;
	/*
	FireDepartment = 3,
	Paramedics = 5,
	Police = 7,
	PedsInCavalcades = 11, 
	Merryweather = 14
	*/
	mIncidentId = 0;
	GAMEPLAY::CREATE_INCIDENT_WITH_ENTITY(14, defender->getPed(),4, 0.0f, &mIncidentId);
	GAMEPLAY::CREATE_INCIDENT_WITH_ENTITY(11, defender->getPed(), 4, 0.0f, &mIncidentId2);

	AI::TASK_USE_MOBILE_PHONE_TIMED(attacker->getPed(), 7000);
	AUDIO::_PLAY_AMBIENT_SPEECH_WITH_VOICE(attacker->getPed(), "PHONE_CALL_COPS", "A_M_M_GOLFER_01_WHITE_MINI_01", "SPEECH_PARAMS_STANDARD",0);
}

bool ChessBattleDeathByCop::isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mIsMovingToSquare) {
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (defender->isPedDeadOrDying()) {
		mIsMovingToSquare = true;
		
		Logger::logDebug("GAMEPLAY::DELETE_INCIDENT");
		GAMEPLAY::DELETE_INCIDENT(&mIncidentId);
		GAMEPLAY::DELETE_INCIDENT(&mIncidentId2);

		attacker->startMovement(chessMove, chessBoard,true);



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

ChessBattleJerryCan::ChessBattleJerryCan(ChessMove chessMove, ChessBoard * chessBoard) : ChessBattle(chessMove, chessBoard)
{
	
	ChessBoardSquare* squareBehind = chessBoard->getSquareBehind(chessMove.getSquareTo(), chessMove.getAttacker()->getSide());
	//only Pawn can use this move with this
	ChessBoardSquare* squareOneStep = chessBoard->getSquareInFrontOf(chessMove.getSquareFrom(), chessMove.getDefender()->getSide());
	ChessBoardSquare* squareTwoStep = chessBoard->getSquareInFrontOf(squareOneStep, chessMove.getDefender()->getSide());

	mCoordsToSneak.push_back(std::make_shared<ActionGoToCoord>(ActionGoToCoord(squareTwoStep->getLocation(), chessMove.getSquareTo()->getHeading(chessMove.getAttacker()->getSide()), 1.0)));
	mCoordsToSneak.push_back(std::make_shared<ActionGoToCoord>(ActionGoToCoord(squareBehind->getLocation(), chessMove.getSquareTo()->getHeading(chessMove.getDefender()->getSide()), 1.0)));
	
	std::shared_ptr<ActionGoToCoord> lastAction = std::make_shared<ActionGoToCoord>(ActionGoToCoord(chessMove.getSquareTo()->getLocation(), chessMove.getSquareTo()->getHeading(chessMove.getAttacker()->getSide()), 1.0));
	lastAction->setMinDistance(1.1);
	mCoordsToSneak.push_back(lastAction);

	mActionShootingLocation = std::make_shared<ActionGoToCoord>(ActionGoToCoord(squareTwoStep->getLocation(), chessMove.getSquareTo()->getHeading(chessMove.getDefender()->getSide()), 1.0));

}

void ChessBattleJerryCan::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	mIsPouring = false;
	Logger::logDebug("ChessBattleJerryCan::startExecution");

	//Make defender ignore gunshots
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(defender->getPed(), true);
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(attacker->getPed(), true);

	STREAMING::REQUEST_CLIP_SET("move_ped_wpn_jerrycan_generic");
	while (!STREAMING::HAS_CLIP_SET_LOADED("move_ped_wpn_jerrycan_generic"))
	{
		WAIT(0);
		if (GetTickCount() - ticksStart > 5000) {
			Logger::logDebug("Failed to load move_ped_wpn_jerrycan_generic");
			return;
		}
	}

	
	mCoordsToSneak[mCoordsToSneakIndex]->start(ticksStart, attacker, defender, chessMove, chessBoard);
}

bool ChessBattleJerryCan::isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mIsShooting) {
		if (AI::GET_SEQUENCE_PROGRESS(attacker->getPed()) == -1) {
			if (!defender->isPedDeadOrDying()) {
				PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
			}

			mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
			return true;
		}
		else if (ticksNow - mTicksStarted > 600) {
			FIRE::START_ENTITY_FIRE(defender->getPed());

			mTicksStarted = mTicksStarted + mTicksStarted;
		}
	}
	else if (mIsPouring) {
		if (AI::GET_SEQUENCE_PROGRESS(attacker->getPed()) == -1) {
			mIsPouring = false;
			mLocationPetrol = ENTITY::GET_ENTITY_COORDS(attacker->getPed(), true);
			mLocationPetrol.z = mLocationPetrol.z - 1.0;
			mActionShootingLocation->start(ticksNow, attacker, defender, chessMove, chessBoard);

			return false;
		}
	}
	else if (mActionShootingLocation->hasBeenStarted()) {
		if (mActionShootingLocation->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard)) {
			mIsShooting = true;
			PED::SET_PED_STEALTH_MOVEMENT(chessMove.getAttacker()->getPed(), 0, 0);

			TaskSequence taskSequence = 100;
			AI::OPEN_SEQUENCE_TASK(&taskSequence);

			chessMove.getAttacker()->equipPrimaryWeapon();
			AI::TASK_SHOOT_AT_COORD(0, mLocationPetrol.x, mLocationPetrol.y, mLocationPetrol.z, 2000, 3337513804);

			AI::CLOSE_SEQUENCE_TASK(taskSequence);
			AI::TASK_PERFORM_SEQUENCE(attacker->getPed(), taskSequence);
			AI::CLEAR_SEQUENCE_TASK(&taskSequence);
			mTicksStarted = ticksNow;
			

			return false;
		}
	}
	else if (mCoordsToSneak[mCoordsToSneakIndex]->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard)) {
		if (mCoordsToSneakIndex == 0) {
			chessMove.getAttacker()->equipWeapon("WEAPON_PETROLCAN");
			PED::SET_PED_WEAPON_MOVEMENT_CLIPSET(chessMove.getAttacker()->getPed(), "move_ped_wpn_jerrycan_generic");

			PED::SET_PED_STEALTH_MOVEMENT(chessMove.getAttacker()->getPed(), 1, 0);
			PED::FORCE_PED_MOTION_STATE(chessMove.getAttacker()->getPed(), 1110276645, false, 0, 0);
			PED::_0x2208438012482A1A(chessMove.getAttacker()->getPed(), 0, 0);
			UNK1::_0x81CBAE94390F9F89();

			PED::SET_PED_MOVEMENT_CLIPSET(chessMove.getAttacker()->getPed(), "move_ped_crouched", 1.0);
		}
		
		mCoordsToSneakIndex++;
		if (mCoordsToSneakIndex < mCoordsToSneak.size()) {
			mCoordsToSneak[mCoordsToSneakIndex]->start(ticksNow, attacker, defender, chessMove, chessBoard);
			return false;
		}
		else {
			mIsPouring = true;
			mPourTaskSequence = 99;
			AI::OPEN_SEQUENCE_TASK(&mPourTaskSequence);

			Logger::logDebug("Pouring petrol for 5000 ms", true);
			AI::TASK_SHOOT_AT_COORD(0, 0.0, 0.0, 0.0, 5000, 3337513804);

			AI::CLOSE_SEQUENCE_TASK(mPourTaskSequence);
			AI::TASK_PERFORM_SEQUENCE(attacker->getPed(), mPourTaskSequence);
			AI::CLEAR_SEQUENCE_TASK(&mPourTaskSequence);
			return false;
		}
	}
	return false;

}

ChessBattleHandToHandWeapon::ChessBattleHandToHandWeapon(ChessMove chessMove, ChessBoard * chessBoard,std::string handToHandWeaponAttacker, std::string handToHandWeaponDefender, bool defenderIsUnarmed, int defenderHealth) : ChessBattle(chessMove, chessBoard)
{
	mHandToHandWeaponAttacker = handToHandWeaponAttacker;
	mHandToHandWeaponDefender = handToHandWeaponDefender;
	mDefenderIsUnarmed = defenderIsUnarmed; 
	mDefenderHealth = defenderHealth;
}

void ChessBattleHandToHandWeapon::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleHandToHandWeapon::startExecution");

	mTicksStarted = ticksStart;
	attacker->removeWeapons();
	attacker->equipWeapon(mHandToHandWeaponAttacker);

	defender->removeWeapons();
	if (!mDefenderIsUnarmed) {
		defender->equipWeapon(mHandToHandWeaponDefender);
	}

	defender->setHealth(mDefenderHealth);

	AI::TASK_COMBAT_PED(attacker->getPed(), defender->getPed(), 0, 16);
	AI::TASK_COMBAT_PED(defender->getPed(), attacker->getPed(), 0, 16);
}

bool ChessBattleHandToHandWeapon::isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mIsMovingToSquare) {
		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (defender->isPedDeadOrDying()) {
		mIsMovingToSquare = true;
		attacker->startMovement(chessMove, chessBoard,true);

		return attacker->isMovementCompleted(chessMove, mNrMovementChecks++);
	}
	else if (ticksNow - mTicksStarted > 20000) {
		Logger::logDebug("ChessBattleHandToHandWeapon::More than 20000 ticks since started.");

		PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
		mTicksStarted = ticksNow;
	}
	return false;
}

ChessBattleFiringSquad::ChessBattleFiringSquad(ChessMove chessMove, ChessBoard * chessBoard, std::string weapon, bool isThrowingWeapon, Animation animation): ChessBattle(chessMove,chessBoard)
{
	mWeapon = weapon;
	mAnimation = animation;
	mIsThrowingWeapon = isThrowingWeapon;
	ChessPiece* attacker = chessMove.getAttacker();
	Ped pedAttacker = attacker->getPed();

	std::array<ChessPiece*, 16> pieces = chessBoard->getChessSet(attacker->getSide())->getPieces();
	for (auto piece : pieces) {
		if (piece->getPed() != pedAttacker) {
			mSquad.push_back(piece);
			piece->setPedCanBeDamaged(false);
		}
	}
	//special case
	if (mSquad.size() == 0) {
		mSquad.push_back(attacker);
	}

	ENTITY::SET_ENTITY_INVINCIBLE(chessMove.getDefender()->getPed(), false);
	ENTITY::SET_ENTITY_PROOFS(chessMove.getDefender()->getPed(), false, false, false, false, false, false, false, false);
	chessMove.getDefender()->setHealth(150);
}

void ChessBattleFiringSquad::startExecution(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{

	for (auto piece : mSquad) {
		piece->equipWeapon(mWeapon);
	}
	
	for (auto piece : mSquad) {
		Vector3 targetLocation = chessMove.getSquareTo()->getLocation();
		if (mIsThrowingWeapon) {
			AI::TASK_THROW_PROJECTILE(piece->getPed(), targetLocation.x, targetLocation.y, targetLocation.z);
		}
		else {
			AI::TASK_SHOOT_AT_ENTITY(piece->getPed(), defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY("FIRING_PATTERN_SINGLE_SHOT"));
		}
	}

	if (mSquad[0]->getPed() != attacker->getPed()) {
		GTAModUtils::playAnimation(attacker->getPed(), mAnimation);
	}
	mTicksStarted = ticksStart;

}

bool ChessBattleFiringSquad::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mActionGoToEndSquare->hasBeenStarted()) {
		return mActionGoToEndSquare->checkForCompletion(ticksNow, attacker,  defender, chessMove, chessBoard);
	}
	else if (defender->isPedDeadOrDying()) {
		/*for (auto piece : mSquad) {
			AI::CLEAR_PED_TASKS(piece->getPed());
		}*/
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
		return false;
	}
	else if (ticksNow - mTicksStarted > 10000) {
		Logger::logDebug("ChessBattleFiringSquad::More than 10000 ticks since started");

		//Kill defender
		PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
	}
	return false;
}

ChessBattleTank::ChessBattleTank(ChessMove chessMove, ChessBoard * chessBoard):ChessBattle(chessMove,chessBoard)
{
}

void ChessBattleTank::startExecution(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleTank::startExecution");
	mTicksStarted = GetTickCount();
	Hash hashTankModel = GAMEPLAY::GET_HASH_KEY("rhino");
	STREAMING::REQUEST_MODEL(hashTankModel);
	while (!STREAMING::HAS_MODEL_LOADED(hashTankModel))
	{
		WAIT(0);
		if (GetTickCount() > mTicksStarted + 5000) {
			//duration will be 0 if it's not loaded
			Logger::logError("ChessBattleTank::startExecution tankModel not loaded after 5000 ticks");
			return;
		}
	}
	Vector3 tankLocation = chessBoard->getVehicleSpawnZone(attacker->getSide());
	float heading = 90.0;
	if (attacker->getSide() == ChessSide::BLACK) {
		heading = 270.0;
	}
	mVehicle = VEHICLE::CREATE_VEHICLE(hashTankModel, tankLocation.x, tankLocation.y, tankLocation.z, heading, 1, 1);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(mVehicle);

	Vector3 otherLocation = ENTITY::GET_ENTITY_COORDS(defender->getPed(), 1);
	heading = GAMEPLAY::GET_HEADING_FROM_VECTOR_2D(otherLocation.x - tankLocation.x, otherLocation.y - tankLocation.y);
	ENTITY::SET_ENTITY_HEADING(mVehicle, heading);


	mActionEnterTank = std::make_shared<ActionEnterVehicle>(ActionEnterVehicle(mVehicle, 2.0f, 15000));
	mActionEnterTank->start(ticksStart, attacker, defender, chessMove, chessBoard);
	mTicksStarted = GetTickCount();
}

bool ChessBattleTank::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mActionGoToEndSquare->hasBeenStarted()) {
		return mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard);
	}
	else if (defender->isPedDeadOrDying()) {
		Logger::logDebug("ChessBattleTank defender is dead has entered tank");
		AI::TASK_LEAVE_VEHICLE(attacker->getPed(), mVehicle, 0);
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
		return false;
	}
	else if (ticksNow - mTicksStarted > 25000) {
		Logger::logDebug("ChessBattleTank::More than 25000 ticks since started");

		//Kill defender
		PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
	}
	else if (mActionEnterTank->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard)) {
		Logger::logDebug("ChessBattleTank attacker has entered tank");
		mTicksStarted = ticksNow;

		TaskSequence taskSequence = 100;
		AI::OPEN_SEQUENCE_TASK(&taskSequence);

		AI::TASK_VEHICLE_AIM_AT_PED(0, defender->getPed());
		AI::TASK_VEHICLE_SHOOT_AT_PED(0, defender->getPed(), 1101004800);

		AI::CLOSE_SEQUENCE_TASK(taskSequence);
		AI::TASK_PERFORM_SEQUENCE(attacker->getPed(), taskSequence);
		AI::CLEAR_SEQUENCE_TASK(&taskSequence);

		//https://www.se7ensins.com/forums/threads/how-to-get-hidden-weapons-like-remote-sniper-in-sp.1174186/
		//WEAPON::SET_CURRENT_PED_VEHICLE_WEAPON(pilot, GAMEPLAY::GET_HASH_KEY("VEHICLE_WEAPON_TANK"));
		VEHICLE::SET_VEHICLE_SHOOT_AT_TARGET(attacker->getPed(), defender->getPed(), 0.0, 0.0, 0.0);

	}

	return false;
}

ChessBattleSlap::ChessBattleSlap(ChessMove chessMove, ChessBoard * chessBoard): ChessBattleManualPlacedSyncedAnimation(chessMove, chessBoard)
{
	mSyncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("takedown_front_slap 2399");
	mSpeedBeforeImpact = 1.0;
}

ChessBattleNeedBiggerGun::ChessBattleNeedBiggerGun(ChessMove chessMove, ChessBoard * chessBoard, std::string weapon1, std::string firingPattern1, std::string weapon2, std::string firingPattern2, DWORD ticksBeforeSwitch): ChessBattle (chessMove,chessBoard)
{
	mWeapon1 = weapon1;
	mFiringPattern1 = firingPattern1;
	mWeapon2 = weapon2;
	mFiringPattern2 = firingPattern2;
	mTicksBeforeSwitch = ticksBeforeSwitch;
}

void ChessBattleNeedBiggerGun::startExecution(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleNeedBiggerGun::startExecution");
	mTicksStarted = ticksStart;

	attacker->equipWeapon(mWeapon1);
	defender->equipWeapon(mWeapon1);

	//defender cannot be damaged with first weapon
	defender->setPedCanBeDamaged(false);


	mFirstWeaponEquipped = true;

	AI::TASK_SHOOT_AT_ENTITY(attacker->getPed(), defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY(strdup(mFiringPattern1.c_str())));
	AI::TASK_SHOOT_AT_ENTITY(defender->getPed(), attacker->getPed(), -1, GAMEPLAY::GET_HASH_KEY(strdup(mFiringPattern1.c_str())));
}

bool ChessBattleNeedBiggerGun::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mActionGoToEndSquare->hasBeenStarted()) {
		return mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard);
	}
	else if (defender->isPedDeadOrDying()) {
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
		return false;
	}
	else if (mFirstWeaponEquipped && ticksNow - mTicksStarted > mTicksBeforeSwitch) {
		defender->setPedCanBeDamaged(true);
		mFirstWeaponEquipped = false;
		attacker->equipWeapon(mWeapon2);
		AI::TASK_SHOOT_AT_ENTITY(attacker->getPed(), defender->getPed(), -1, GAMEPLAY::GET_HASH_KEY(strdup(mFiringPattern2.c_str())));
		mTicksStarted = ticksNow;
	}
	else if (ticksNow - mTicksStarted > 20000) {
		Logger::logDebug("ChessBattleNeedBiggerGun::More than 25000 ticks since started");

		//Kill defender
		PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
	}


	return false;
}

ChessBattleTurnIntoAnimal::ChessBattleTurnIntoAnimal(ChessMove chessMove, ChessBoard * chessBoard, std::string animalModel, Animation animation, DWORD waitTimeAfterAnimation): ChessBattle(chessMove,chessBoard)
{
	mAnimalModelName = animalModel;
	mAnimation = animation;
	mWaitTimeAfterAnimation = waitTimeAfterAnimation;
}

void ChessBattleTurnIntoAnimal::startExecution(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleTurnIntoAnimal::startExecution");
	mTicksStarted = ticksStart;
	
	mAnimalModelHash = GAMEPLAY::GET_HASH_KEY(strdup(mAnimalModelName.c_str()));
	STREAMING::REQUEST_MODEL(mAnimalModelHash);
	while (!STREAMING::HAS_MODEL_LOADED(mAnimalModelHash)) {
		WAIT(0);
		if (GetTickCount() - ticksStart > 15000) {
			Logger::logDebug("Failed to load " + mAnimalModelName);
			break;
		}
	}
	
	attacker->forceFaceHeadingTowards(defender);
	GTAModUtils::playAnimation(attacker->getPed(), mAnimation);
	mIsWaitingAfterAnimation = true;
}

bool ChessBattleTurnIntoAnimal::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mActionGoToEndSquare->hasBeenStarted()) {
		if (mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard)) {
			return true;
		}
	}
	else if (!mIsWaitingAfterAnimation) {
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
		return false;
	}
	else if (mIsWaitingAfterAnimation && ticksNow - mTicksStarted > mWaitTimeAfterAnimation) {
		mIsWaitingAfterAnimation = false;
		Vector3 location = ENTITY::GET_ENTITY_COORDS(defender->getPed(), 1);
		float heading = ENTITY::GET_ENTITY_HEADING(defender->getPed());

		

		mAnimalPed = PED::CREATE_PED(28, mAnimalModelHash, location.x, location.y, location.z, heading, false, true);
		defender->removePed();
		defender->setPed(mAnimalPed);

		Vector3 fleeLocation = chessBoard->getVehicleSpawnZone(defender->getSide());
		AI::TASK_GO_STRAIGHT_TO_COORD(mAnimalPed, fleeLocation.x, fleeLocation.y, fleeLocation.z, 2.0, -1, heading, 0.0);
		mTicksStarted = ticksNow;
	}


	return false;
}

ChessBattleShootOut::ChessBattleShootOut(ChessMove chessMove, ChessBoard * chessBoard): ChessBattleManualPlacedSyncedAnimation(chessMove,chessBoard)
{
	mSyncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("Shoot out");
	mSpeedBeforeImpact = 1.0;
	mForceHeading = true;
}


void ChessBattleShootOut::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleShootOut::startExecution " + mSyncedAnimation->getTitle());

	attacker->setPedFreezed(true);

	attacker->equipPrimaryWeapon();
	defender->equipPrimaryWeapon();

	attacker->forceFaceHeadingTowards(defender);
	defender->forceFaceHeadingTowards(attacker);

	//mIsMovingIntoPosition = true;
	//mSquareSetup = chessBoard->getSquareInFrontOf(chessMove.getSquareTo(), attacker->getSide());
	//mSquareSetup = chessBoard->getSquareInFrontOf(mSquareSetup, attacker->getSide());
	//Vector3 squareToLocation = mSquareSetup->getLocation();
	//AI::TASK_GO_STRAIGHT_TO_COORD(attacker->getPed(), squareToLocation.x, squareToLocation.y, squareToLocation.z, 1.0, -1, attacker->getSide(), 0.5f);
	std::vector<ChessPiece*> actors = { attacker, defender };
	Vector3 startLocation = ENTITY::GET_ENTITY_COORDS(attacker->getPed(), true);
	mSyncedAnimation->executeSyncedAnimation(true, actors, false, startLocation, false, true, false);
	mHasDefenderFired = false;
	mHasAttackerFired = false;
}

bool ChessBattleShootOut::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mActionGoToEndSquare->hasBeenStarted()) {
		return mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard);
	}
	else if (mSyncedAnimation->isCompleted()) {
		mSyncedAnimation->cleanupAfterExecution(true, false);
		if (!defender->isPedDeadOrDying()) {
			defender->removePed();
		}
		attacker->setPedFreezed(false);
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
	}	
	Logger::logDebug("mSyncedAnimation->getProgress() " +std::to_string(mSyncedAnimation->getProgress()));

	if (!mHasAttackerFired && mSyncedAnimation->getProgress() >= 0.6 &&mSyncedAnimation->getProgress() <= 0.7) {
		Logger::logDebug("ChessBattleShootOut attacker firing: " );
		Vector3 weaponImpact = ENTITY::GET_ENTITY_COORDS(defender->getPed(), 1);
		PED::SET_PED_SHOOTS_AT_COORD(attacker->getPed(), weaponImpact.x, weaponImpact.y, weaponImpact.z, 1);
		mHasAttackerFired = true;
	}

	if (!mHasDefenderFired && mSyncedAnimation->getProgress() >= 0.4 &&mSyncedAnimation->getProgress() <= 0.5) {
		Logger::logDebug("ChessBattleShootOut defender firing");
		Vector3 weaponImpact = ENTITY::GET_ENTITY_COORDS(attacker->getPed(), 1);
		PED::SET_PED_SHOOTS_AT_COORD(defender->getPed(), weaponImpact.x, weaponImpact.y, weaponImpact.z, 1);
		mHasDefenderFired = true;
	}


	return false;
}

ChessBattleHatchetFront::ChessBattleHatchetFront(ChessMove chessMove, ChessBoard * chessBoard) : ChessBattleManualPlacedSyncedAnimation(chessMove, chessBoard)
{
	mSyncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("Hatchet takedown front");
	mSpeedBeforeImpact = 1.0;
	mForceHeading = true;
	mTicksBeforeAction = 1100;
}

void ChessBattleHatchetFront::startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleHatchetFront::startExecution " + mSyncedAnimation->getTitle());

	//1. Move to square ahead of defender
	//2. Start running to square of defender
	//3. After a few hundred ms trigger synched anim
	//4. Wait for synched anim to complete
	//5. Move to target square
	//6. Wait for getting close to target square
	attacker->equipWeapon("WEAPON_HATCHET");

	mIsMovingIntoPosition = true;
	mSquareSetup = chessBoard->getSquareInFrontOf(chessMove.getSquareTo(), attacker->getSide());
	Vector3 squareToLocation = mSquareSetup->getLocation();
	AI::TASK_GO_STRAIGHT_TO_COORD(attacker->getPed(), squareToLocation.x, squareToLocation.y, squareToLocation.z, 1.0, -1, attacker->getSide(), 0.5f);
}

ChessBattleThrowGrenade::ChessBattleThrowGrenade(ChessMove chessMove, ChessBoard * chessBoard, std::string grenadeName):ChessBattle(chessMove,chessBoard)
{
	mGrenadeName = grenadeName;
	mActionThrowGrenade = std::make_shared<ActionThrowGrenade>(ActionThrowGrenade(grenadeName, 7000));
	
}

void ChessBattleThrowGrenade::startExecution(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	attacker->equipWeapon(mGrenadeName);
	mTicksStarted = ticksStart;
}

bool ChessBattleThrowGrenade::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{

	if (mActionGoToEndSquare->hasBeenStarted()) {
		if (mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard)) {
			return true;
		}
	}
	else if (defender->isPedDeadOrDying()) {
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
		return false;
	}
	else if (mActionThrowGrenade->hasBeenStarted()) {
		if (mActionThrowGrenade->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard)) {
			if (!defender->isPedDeadOrDying()) {
				//Kill defender
				PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
			}
			return false;
		}
	}
	else if (!mActionThrowGrenade->hasBeenStarted() && ticksNow - mTicksStarted > 700) {
		mActionThrowGrenade->start(ticksNow, attacker, defender, chessMove, chessBoard);
		return false;
	}

	return false;
}

ChessBattleShoryuken::ChessBattleShoryuken(ChessMove chessMove, ChessBoard * chessBoard):ChessBattleManualPlacedSyncedAnimation(chessMove,chessBoard)
{
	//default to be overriddden by subclasses
	mSyncedAnimation = chessBoard->getSyncedAnimationFactory()->getByTitle("Melee takedown front uppercut");
	mSpeedBeforeImpact = 1.0;
	mForceHeading = true;
	mTicksBeforeAction = 650;
	mDoneMegaPunch = false;

}

bool ChessBattleShoryuken::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (!mDoneMegaPunch & mSyncedAnimation->getProgress() >= 0.23) {
		Logger::logDebug("ChessBattleShoryuken mega punch");

		Vector3 force = ENTITY::GET_ENTITY_FORWARD_VECTOR(attacker->getPed());
		//Vector3 force;
		float sideMultiplier = 1.0f;
		if (attacker->getSide() == ChessSide::BLACK) {
			sideMultiplier = -1.0f;
		}
		force.x = force.x* 0.8 * 1000.0;
		force.y = force.y* 0.8 * 1000.0;
		force.z = 0.3 * 1000.0;
		Vector3 rotation;

		Logger::logDebug("Force " + std::to_string(force.x) + "," + std::to_string(force.y) + "," + std::to_string(force.z) + ",");

		//AI::CLEAR_PED_TASKS(defender->getPed());

		ENTITY::APPLY_FORCE_TO_ENTITY(defender->getPed(), 3, force.x, force.y, force.z, rotation.x, rotation.y, rotation.z, false, false, true, true, false, true);

		//PED::SET_PED_TO_RAGDOLL(defender->getPed(), 5000, 5000, 0, false, false, false);
		//defender->setHealth(0);
		mDoneMegaPunch = true;
		mDoneRagdoll = false;
		mTicksStarted = ticksNow;
	}
	if (!mDoneRagdoll && mDoneMegaPunch && ticksNow - mTicksStarted > 200) {
		Logger::logDebug("ChessBattleShoryuken ragdoll");
		mDoneRagdoll = true;
		AI::SET_HIGH_FALL_TASK(defender->getPed(), 10000, 30000, 0);
		//defender->setHealth(0);
		AUDIO::_PLAY_AMBIENT_SPEECH_WITH_VOICE(defender->getPed(), "SCREAM_TERROR", "WAVELOAD_PAIN_MALE", "SPEECH_PARAMS_STANDARD", 0);
		//PED::SET_PED_TO_RAGDOLL(defender->getPed(), 10000, 10000, 0, false, false, false);
	}


	if (mActionGoToEndSquare->hasBeenStarted()) {
		if (mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard)) {
			defender->setHealth(0);
			return true;
		}
	}
	else if (mIsMovingIntoPosition) {

		Vector3 currentLocation = ENTITY::GET_ENTITY_COORDS(attacker->getPed(), true);
		Vector3 squareToLocation = mSquareSetup->getLocation();
		float distanceToTarget = SYSTEM::VDIST(squareToLocation.x, squareToLocation.y, squareToLocation.z, currentLocation.x, currentLocation.y, currentLocation.z);

		if (distanceToTarget < 1.5) {
			mIsMovingIntoPosition = false;
			Vector3 squareToLocation = chessMove.getSquareTo()->getLocation();
			AI::TASK_GO_STRAIGHT_TO_COORD(attacker->getPed(), squareToLocation.x, squareToLocation.y, squareToLocation.z, mSpeedBeforeImpact, -1, mSquareSetup->getHeading(attacker->getSide()), 0.5f);
			mIsWaitingForTriggeringSynced = true;
			mTicksStarted = ticksNow;
		}
		return false;
	}
	else if (mIsWaitingForTriggeringSynced) {
		if (ticksNow - mTicksStarted > mTicksBeforeAction) {
			mIsWaitingForTriggeringSynced = false;

			std::vector<ChessPiece*> actors = { attacker };

			Vector3 startLocation = ENTITY::GET_ENTITY_COORDS(defender->getPed(), true);

			startLocation.x = startLocation.x + 0.0;
			startLocation.y = startLocation.y + 0.0;
			startLocation.z = startLocation.z + 0.0;
			if (mForceHeading) {
				ENTITY::SET_ENTITY_HEADING(attacker->getPed(), mSquareSetup->getHeading(attacker->getSide()));
			}
			mSyncedAnimation->executeSyncedAnimation(true, actors, false, startLocation, false, true, false);
			//Animation defenderAnimation = chessBoard->getAnimationFactory()->getAnimationForShortcutIndex(6702);
			//GTAModUtils::playAnimation(defender->getPed(), defenderAnimation);
		}
		return false;
	}
	else if (mSyncedAnimation->isCompleted() && ticksNow - mTicksStarted > 65005) {
		mSyncedAnimation->cleanupAfterExecution(true, false);
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
		return false;
	}



	return false;
}

ChessBattleAttackedByAnimals::ChessBattleAttackedByAnimals(ChessMove chessMove, ChessBoard * chessBoard, std::string animalName, int nrOfAnimalsPrSide): ChessBattle(chessMove,chessBoard)
{
	mNrOfAnimalsPrSide = nrOfAnimalsPrSide;
	mAnimalName = animalName;
}

void ChessBattleAttackedByAnimals::startExecution(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleAttackedByAnimals::startExecution");

	Hash mAnimalHash = GAMEPLAY::GET_HASH_KEY(strdup(mAnimalName.c_str()));
	STREAMING::REQUEST_MODEL(mAnimalHash);
	while (!STREAMING::HAS_MODEL_LOADED(mAnimalHash)) {
		if (GetTickCount() - ticksStart > 5000) {
			Logger::logDebug("Failed to load " + mAnimalName);
			return;
		}
		WAIT(0);
	}
	//05089 facials@p_m_zero@variations@elkcall mood_elkcal_1 3633
	Animation elkCallAnimation = chessBoard->getAnimationFactory()->getAnimationForShortcutIndex(5089);
	GTAModUtils::playAnimation(attacker->getPed(), elkCallAnimation);

	//Audio.PlaySoundFromEntity("Franklin_Whistle_For_Chop", Game.Player.Character, "SPEECH_RELATED_SOUNDS");
	AUDIO::PLAY_SOUND_FROM_ENTITY(-1, "Franklin_Whistle_For_Chop", attacker->getPed(), "SPEECH_RELATED_SOUNDS", 0, 0);

	//TODO player.Task.PlayAnimation("facials@p_m_zero@variations@elkcall", "mood_elkcal_1");

	Vector3 otherLocation = ENTITY::GET_ENTITY_COORDS(defender->getPed(), 1);
	Vector3 location = chessBoard->getVehicleSpawnZone(ChessSide::WHITE);
	float heading = GAMEPLAY::GET_HEADING_FROM_VECTOR_2D(otherLocation.x - location.x, otherLocation.y - location.y);
	for (int i = 0; i < mNrOfAnimalsPrSide *2 ; i++) {
		if (i == mNrOfAnimalsPrSide) {
			location = chessBoard->getVehicleSpawnZone(ChessSide::BLACK);
			heading = GAMEPLAY::GET_HEADING_FROM_VECTOR_2D(otherLocation.x - location.x, otherLocation.y - location.y);
		}
		Ped animalPed = PED::CREATE_PED(4, mAnimalHash, location.x, location.y, location.z, heading, false, true);
		//never flee
		PED::SET_PED_COMBAT_ATTRIBUTES(animalPed, 46, true);
		//fight with melee weapons
		PED::SET_PED_COMBAT_ATTRIBUTES(animalPed, 5, true);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(animalPed, false);
		ENTITY::SET_ENTITY_INVINCIBLE(animalPed,true);
		ENTITY::SET_ENTITY_PROOFS(animalPed, 1, 1, 1, 1, 1, 1, 1, 1);
		PED::SET_PED_SEEING_RANGE(animalPed, 100.0f);
		PED::SET_PED_HEARING_RANGE(animalPed, 100.0f);
		PED::SET_PED_ALERTNESS(animalPed, 3);
		PED::SET_PED_COMBAT_RANGE(animalPed, 2);
		mAnimals.push_back(animalPed);
	}

	for (auto animal : mAnimals) {
		AI::TASK_COMBAT_PED(animal, defender->getPed(), 0, 16);
	}
	mIsWaitingForAnimalsToAttack = true;
	mRemovedAnimals = false;
	mTicksStarted = ticksStart;
}

bool ChessBattleAttackedByAnimals::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mActionGoToEndSquare->hasBeenStarted()) {
		if (mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard) &&  ticksNow - mTicksStarted > 15000) {
			if (!mRemovedAnimals) {
				Logger::logDebug("ChessBattleAttackedByAnimals Removing animals");
				mRemovedAnimals = true;
				for (auto animal : mAnimals) {
					if (ENTITY::DOES_ENTITY_EXIST(animal))
					{
						PED::DELETE_PED(&animal);
					}
				}
			}

			return true;
		}
		else {
			return false;
		}
	}
	else if (defender->isPedDeadOrDying()) {
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
		mIsWaitingForAnimalsToAttack = false;

		for (auto animal : mAnimals) {
			Vector3 location = chessBoard->getVehicleSpawnZone(ChessSide::WHITE);
			AI::TASK_GO_STRAIGHT_TO_COORD(animal, location.x, location.y, location.z, 1.0, -1, 1.0, 0.0);
		}
		mTicksStarted = ticksNow;
		return false;
	}
	else if (mIsWaitingForAnimalsToAttack && ticksNow - mTicksStarted > 20000) {
		Logger::logDebug("ChessBattleAttackedByAnimals killing defender after 20000 ticks");
		PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);
		return false;
	}
}

ChessBattleAttackOfTheClones::ChessBattleAttackOfTheClones(ChessMove chessMove, ChessBoard * chessBoard):ChessBattle(chessMove, chessBoard)
{
	mAnimation = chessBoard->getAnimationFactory()->getAnimationForShortcutIndex(2925);
}

void ChessBattleAttackOfTheClones::startExecution(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleAttackOfTheClones::startExecution");

	mTicksStarted = ticksStart;
	mIsCloning = true;

	Vector3 location = chessMove.getSquareTo()->getLocation();
	location.x = location.x - 2.0;

	defender->setHealth(135);

	GTAModUtils::playAnimation(attacker->getPed(), mAnimation);
	createClone(attacker, location, ENTITY::GET_ENTITY_COORDS(defender->getPed(), true));
	mCloningIndex++;
	mWaitingForClonesToAttack = false;
	defender->equipSecondaryWeapon();
}

bool ChessBattleAttackOfTheClones::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mActionGoToEndSquare->hasBeenStarted()) {
		if (mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard) && ticksNow - mTicksStarted > 15000) {
			if (!mHasRemovedClones) {
				Logger::logDebug("ChessBattleAttackOfTheClones Removing clones");
				mHasRemovedClones = true;
				for (auto clone : mClones) {
					if (ENTITY::DOES_ENTITY_EXIST(clone))
					{
						PED::DELETE_PED(&clone);
					}
				}
			}

			return true;
		}
		else {
			return false;
		}
	}
	else if (defender->isPedDeadOrDying()) {
		Logger::logDebug("ChessBattleAttackOfTheClones isPedDeadOrDying");
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
		mWaitingForClonesToAttack = false;

		for (auto clone : mClones) {
			Vector3 location = chessBoard->getVehicleSpawnZone(ChessSide::WHITE);
			AI::TASK_GO_STRAIGHT_TO_COORD(clone, location.x, location.y, location.z, 1.0, -1, 1.0, 0.0);
		}
		mTicksStarted = ticksNow;
		return false;
	}
	else if (mWaitingForClonesToAttack && ticksNow - mTicksStarted > 1500) {
		Logger::logDebug("ChessBattleAttackOfTheClones mWaitingForClonesToAttack");
		//PED::EXPLODE_PED_HEAD(defender->getPed(), 0x5FC3C11);

		for (auto clone : mClones) {
			AI::TASK_PUT_PED_DIRECTLY_INTO_MELEE(clone, defender->getPed(), 0.0, -1.0, 0.0, 0);
		}
		
		mWaitingForClonesToAttack = false;
		return false;
	}
	else if (mIsCloning && ticksNow - mTicksStarted > 3000) {
		Logger::logDebug("ChessBattleAttackOfTheClones mIsCloning " + std::to_string(mCloningIndex));
		Vector3 location = chessMove.getSquareTo()->getLocation();
		if (mCloningIndex == 1) {
			location.y = location.y + 2.0;
		}
		else if(mCloningIndex == 2){
			location.x = location.x + 2.0;
		}
		GTAModUtils::playAnimation(attacker->getPed(), mAnimation);
		createClone(attacker, location, ENTITY::GET_ENTITY_COORDS(defender->getPed(),true));
		mCloningIndex++;
		if (mCloningIndex >= 3) {
			mIsCloning = false;
			mWaitingForClonesToAttack = true;
		}

		mTicksStarted = ticksNow;
	}

	return false;
}

void ChessBattleAttackOfTheClones::createClone(ChessPiece* chessPiece, Vector3 location, Vector3 faceLocation)
{
	float heading = heading = GAMEPLAY::GET_HEADING_FROM_VECTOR_2D(faceLocation.x - location.x, faceLocation.y - location.y);
	Ped clonedPed = PED::CLONE_PED(chessPiece->getPed(), heading, false, true);

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(clonedPed, location.x, location.y, location.z, 0, 0, 1);
	ENTITY::SET_ENTITY_HEADING(clonedPed, heading);
	mClones.push_back(clonedPed);
}

ChessBattleSharkAttack::ChessBattleSharkAttack(ChessMove chessMove, ChessBoard * chessBoard): ChessBattle(chessMove,chessBoard)
{
	chessBoard->getSyncedAnimationFactory()->getByTitle("Shark attack");
	mAnimation = chessBoard->getAnimationFactory()->getAnimationForShortcutIndex(2925);
}

void ChessBattleSharkAttack::startExecution(DWORD ticksStart, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	Logger::logDebug("ChessBattleSharkAttack::startExecution " + mSyncedAnimation->getTitle());
	
	mAnimalHash = GAMEPLAY::GET_HASH_KEY("a_c_sharktiger");
	STREAMING::REQUEST_MODEL(mAnimalHash);
	while (!STREAMING::HAS_MODEL_LOADED(mAnimalHash)) {
		if (GetTickCount() - ticksStart > 5000) {
			Logger::logDebug("Failed to load a_c_sharktiger");
			return;
		}
		WAIT(0);
	}

	GTAModUtils::playAnimation(attacker->getPed(), mAnimation);
	mTicksStarted = ticksStart;
}

bool ChessBattleSharkAttack::isExecutionCompleted(DWORD ticksNow, ChessPiece * attacker, ChessPiece * defender, ChessMove chessMove, ChessBoard * chessBoard)
{
	if (mActionGoToEndSquare->hasBeenStarted()) {
		return mActionGoToEndSquare->checkForCompletion(ticksNow, attacker, defender, chessMove, chessBoard);
	}
	else if (mSyncedAnimation->isActive() && mSyncedAnimation->isCompleted()) {
		Logger::logDebug("ChessBattleAttackOfTheClones mSyncedAnimation is complete");

		mSyncedAnimation->cleanupAfterExecution(true, false);

		defender->removePed();
		if (ENTITY::DOES_ENTITY_EXIST(mSharkPed))
		{
			PED::DELETE_PED(&mSharkPed);
		}
		mActionGoToEndSquare->start(ticksNow, attacker, defender, chessMove, chessBoard);
	}
	else if (ticksNow - mTicksStarted > 4000) {
		Logger::logDebug("ChessBattleAttackOfTheClones creating shark");
		Vector3 startLocation = ENTITY::GET_ENTITY_COORDS(defender->getPed(), true);

		mSharkPed = PED::CREATE_PED(4, mAnimalHash, startLocation.x, startLocation.y, startLocation.z+20.0, 0.0f, false, true);
		//never flee
		PED::SET_PED_COMBAT_ATTRIBUTES(mSharkPed, 46, true);
		//fight with melee weapons
		PED::SET_PED_COMBAT_ATTRIBUTES(mSharkPed, 5, true);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(mSharkPed, false);
		ENTITY::SET_ENTITY_INVINCIBLE(mSharkPed, true);

		std::vector<Ped> peds = { mSharkPed, defender->getPed() };

		mSyncedAnimation->executeSyncedAnimation(true, peds, false, startLocation, false, true, false);
	}

	return false;



}
