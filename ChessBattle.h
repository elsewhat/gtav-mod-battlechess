#pragma once

#include "inc\types.h"
class ActionGoToSquare;
class ActionGoToCoord;
class ActionEnterVehicle;
class ActionThrowGrenade;
#include "ChessBattleAction.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
class SyncedAnimation;
#include "SyncedAnimation.h"
class Animation;
#include "Animation.h"
#include <vector>
#include <memory>

class ChessBattle {
public:
	ChessBattle(ChessMove chessMove, ChessBoard* chessBoard);

	virtual void initializeBattle(ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard);
	virtual void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)=0;
	virtual bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove,ChessBoard* chessBoard)=0;

	void setMessage(std::string msg, DWORD ticks);
	void drawMessage(ChessBoard* chessBoard);

protected:
	std::shared_ptr<ActionGoToSquare> mActionGoToEndSquare;
	DWORD mTicksStarted;
	bool mIsMovingToSquare;
	int mNrMovementChecks;
	int mScaleformBigMessage;
	std::string mMsgBattle;
	DWORD mMsgTicksStarted;
	DWORD mMsgTicksLength;
};



class ChessBattleFirePrimaryWeapon : public ChessBattle {
public:
	ChessBattleFirePrimaryWeapon(ChessMove chessMove, ChessBoard* chessBoard);
	ChessBattleFirePrimaryWeapon(ChessMove chessMove, ChessBoard* chessBoard,std::string firingPattern);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	std::string mFiringPattern;
	virtual void equipWeapon(ChessMove chessMove);
};

class ChessBattleFireSecondaryWeapon : public ChessBattleFirePrimaryWeapon {
public:
	ChessBattleFireSecondaryWeapon(ChessMove chessMove, ChessBoard* chessBoard);
	ChessBattleFireSecondaryWeapon(ChessMove chessMove, ChessBoard* chessBoard,std::string firingPattern);

protected:
	void equipWeapon(ChessMove chessMove)override;
};

class ChessBattleNeedBiggerGun: public ChessBattle {
public:
	ChessBattleNeedBiggerGun(ChessMove chessMove, ChessBoard* chessBoard, std::string weapon1, std::string firingPattern1, std::string weapon2, std::string firingPattern2, DWORD ticksBeforeSwitch);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mFirstWeaponEquipped;
	std::string mWeapon1;
	std::string mFiringPattern1;
	std::string mWeapon2; 
	std::string mFiringPattern2;
	DWORD mTicksBeforeSwitch;
};

class ChessBattleSyncedAnimation : public ChessBattle {
public:
	ChessBattleSyncedAnimation(ChessMove chessMove, ChessBoard* chessBoard,std::shared_ptr<SyncedAnimation> syncedAnimation, bool killAfterwards, bool useDefenderLocation, Vector3 locationOffset);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mKillAfterwards;
	bool mUseDefenderLocation;
	Vector3 mLocationOffset;
	std::shared_ptr<SyncedAnimation> mSyncedAnimation;
};



class ChessBattleSyncedAnimationChained : public ChessBattle {
public:
	ChessBattleSyncedAnimationChained(ChessMove chessMove, ChessBoard* chessBoard,std::shared_ptr<SyncedAnimation> firstSyncedAnimation, std::shared_ptr<SyncedAnimation> secondSyncedAnimation, bool killAfterwards, bool useDefenderLocation, Vector3 locationOffset, bool hasDefenderWeapon, std::string defenderWeapon);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mKillAfterwards;
	bool mUseDefenderLocation;
	bool mIsFirstCompleted;
	bool mHasDefenderWeapon;
	std::string mDefenderWeapon;
	Vector3 mLocationOffset;
	Vector3 mStartLocation;
	std::shared_ptr<SyncedAnimation> mFirstSyncedAnimation;
	std::shared_ptr<SyncedAnimation> mSecondSyncedAnimation;
};

class ChessBattleKnifeStruggle : public ChessBattleSyncedAnimationChained {
public:
	ChessBattleKnifeStruggle(ChessMove chessMove, ChessBoard* chessBoard, std::shared_ptr<SyncedAnimation> firstSyncedAnimation, std::shared_ptr<SyncedAnimation> secondSyncedAnimation, bool killAfterwards, bool useDefenderLocation, Vector3 locationOffset, bool hasDefenderWeapon, std::string defenderWeapon);

	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
};


class ChessBattleManualPlacedSyncedAnimation : public ChessBattle {
public:
	ChessBattleManualPlacedSyncedAnimation(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mForceHeading;
	DWORD mTicksBeforeAction;
	float mSpeedBeforeImpact;
	ChessBoardSquare* mSquareSetup;
	bool mIsMovingIntoPosition;
	bool mIsWaitingForTriggeringSynced;
	std::shared_ptr<SyncedAnimation> mSyncedAnimation;
};

class ChessBattleHeadbutt : public ChessBattleManualPlacedSyncedAnimation {
public:
	ChessBattleHeadbutt(ChessMove chessMove, ChessBoard* chessBoard);
protected:
};

class ChessBattleSlap : public ChessBattleManualPlacedSyncedAnimation {
public:
	ChessBattleSlap(ChessMove chessMove, ChessBoard* chessBoard);

protected:
};

class ChessBattleShootOut : public ChessBattleManualPlacedSyncedAnimation {
public:
	ChessBattleShootOut(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mHasDefenderFired;
	bool mHasAttackerFired;
};

class ChessBattleShoryuken : public ChessBattleManualPlacedSyncedAnimation {
public:
	ChessBattleShoryuken(ChessMove chessMove, ChessBoard* chessBoard);

	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mDoneMegaPunch;
	bool mDoneRagdoll;
};


class ChessBattleHatchetFront : public ChessBattleManualPlacedSyncedAnimation {
public:
	ChessBattleHatchetFront(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
protected:
};


class ChessBattleStealthKill : public ChessBattle {
public:
	ChessBattleStealthKill(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
};

class ChessBattleDeathByCop : public ChessBattle {
public:
	ChessBattleDeathByCop(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	Any mIncidentId;
	Any mIncidentId2;
};

class ChessBattleJerryCan : public ChessBattle {
public:
	ChessBattleJerryCan(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	std::vector<std::shared_ptr<ActionGoToCoord>> mCoordsToSneak;
	std::shared_ptr<ActionGoToCoord> mActionShootingLocation;
	Vector3 mLocationPetrol;
	int mCoordsToSneakIndex = 0;
	bool mIsPouring;
	bool mIsShooting;
	TaskSequence mPourTaskSequence;
};

class ChessBattleHandToHandWeapon : public ChessBattle {
public:
	ChessBattleHandToHandWeapon(ChessMove chessMove, ChessBoard* chessBoard,std::string handToHandWeaponAttacker, std::string handToHandWeaponDefender, bool defenderIsUnarmed, int defenderHealth);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	std::string mHandToHandWeaponAttacker;
	std::string mHandToHandWeaponDefender;
	bool mDefenderIsUnarmed;
	int mDefenderHealth;
};

class ChessBattleFiringSquad : public ChessBattle {
public:
	ChessBattleFiringSquad(ChessMove chessMove, ChessBoard* chessBoard, std::string weapon, bool isThrowingWeapon, Animation animation);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	std::string mWeapon;
	bool mIsThrowingWeapon;
	std::vector<ChessPiece*> mSquad;
	Animation mAnimation;
};


class ChessBattleTank : public ChessBattle {
public:
	ChessBattleTank(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	Vehicle mVehicle;
	std::shared_ptr<ActionEnterVehicle> mActionEnterTank;
};

class ChessBattleTurnIntoAnimal: public ChessBattle {
public:
	ChessBattleTurnIntoAnimal(ChessMove chessMove, ChessBoard* chessBoard, std::string animalModel, Animation animation, DWORD waitTimeAfterAnimation);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	Ped mAnimalPed;
	std::string mAnimalModelName;
	DWORD mAnimalModelHash;
	Animation mAnimation;
	DWORD mWaitTimeAfterAnimation;
	bool mIsWaitingAfterAnimation;
};

class ChessBattleThrowGrenade : public ChessBattle {
public:
	ChessBattleThrowGrenade(ChessMove chessMove, ChessBoard* chessBoard, std::string grenadeName);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	std::string mGrenadeName;
	std::shared_ptr<ActionThrowGrenade> mActionThrowGrenade;
};

class ChessBattleAttackedByAnimals : public ChessBattle {
public:
	ChessBattleAttackedByAnimals(ChessMove chessMove, ChessBoard* chessBoard, std::string animalName, int nrOfAnimalsPrSide);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	std::vector<Ped> mAnimals;
	int mNrOfAnimalsPrSide;
	std::string mAnimalName;
	bool mIsWaitingForAnimalsToAttack;
	bool mRemovedAnimals;
};

class ChessBattleAttackOfTheClones: public ChessBattle {
public:
	ChessBattleAttackOfTheClones(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

	void createClone(ChessPiece* chessPiece, Vector3 location, Vector3 faceLocation);

protected:
	std::vector<Ped> mClones;
	Animation mAnimation;
	int mNrOfClones;
	bool mIsCloning;
	int mCloningIndex;
	bool mHasRemovedClones;
	bool mWaitingForClonesToAttack;
};

class ChessBattleSharkAttack : public ChessBattle {
public:
	ChessBattleSharkAttack(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	std::shared_ptr<SyncedAnimation> mSyncedAnimation;
	Hash mAnimalHash;
	Ped mSharkPed;
	bool mIsWaitingForAttack;
	Animation mAnimation;
};

class ChessBattleDeathByMusician : public ChessBattle {
public:
	ChessBattleDeathByMusician(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mIsWaitingForSuicide;
	Animation mAnimationSuicide;
};

class ChessBattleDeathByYoga : public ChessBattle {
public:
	ChessBattleDeathByYoga(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	Animation mAnimationDeath;
	bool mIsWaitingForYoga;
	bool mIsWaitingForDeath;
};

class ChessBattleScaredByFlex : public ChessBattle {
public:
	ChessBattleScaredByFlex(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mIsWaitingForFlee;
};

class ChessBattleMagicTouch : public ChessBattle {
public:
	ChessBattleMagicTouch(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mIsWaiting;
	Animation mAnimationMagicTouch;
};

class ChessBattleMagicUndress : public ChessBattle {
public:
	ChessBattleMagicUndress(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mIsWaiting;
	bool mIsChanging;
	Animation mAnimationMagicTouch;
	int mChanges;
	DWORD mChangesDelta;
};