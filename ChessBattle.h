#pragma once

#include "inc\types.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
class SyncedAnimation;
#include "SyncedAnimation.h"
#include <vector>
#include <memory>

class ChessBattle {
public:
	ChessBattle();

	virtual void initializeBattle(ChessMove chessMove, ChessBoard* chessBoard);
	virtual void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)=0;
	virtual bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove,ChessBoard* chessBoard)=0;

protected:
	DWORD mTicksStarted;
	bool mIsMovingToSquare;
	int mNrMovementChecks;
};



class ChessBattleFirePrimaryWeapon : public ChessBattle {
public:
	ChessBattleFirePrimaryWeapon();
	ChessBattleFirePrimaryWeapon(std::string firingPattern);

	void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	std::string mFiringPattern;
	virtual void equipWeapon(ChessMove chessMove);
};

class ChessBattleFireSecondaryWeapon : public ChessBattleFirePrimaryWeapon {
public:
	ChessBattleFireSecondaryWeapon();
	ChessBattleFireSecondaryWeapon(std::string firingPattern);

protected:
	void equipWeapon(ChessMove chessMove)override;
};

class ChessBattleSyncedAnimation : public ChessBattle {
public:
	ChessBattleSyncedAnimation(std::shared_ptr<SyncedAnimation> syncedAnimation, bool killAfterwards, bool useDefenderLocation, Vector3 locationOffset);

	void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mKillAfterwards;
	bool mUseDefenderLocation;
	Vector3 mLocationOffset;
	std::shared_ptr<SyncedAnimation> mSyncedAnimation;
};

class ChessBattleSyncedAnimationChained : public ChessBattle {
public:
	ChessBattleSyncedAnimationChained(std::shared_ptr<SyncedAnimation> firstSyncedAnimation, std::shared_ptr<SyncedAnimation> secondSyncedAnimation, bool killAfterwards, bool useDefenderLocation, Vector3 locationOffset);

	void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	bool mKillAfterwards;
	bool mUseDefenderLocation;
	bool mIsFirstCompleted;
	Vector3 mLocationOffset;
	Vector3 mStartLocation;
	std::shared_ptr<SyncedAnimation> mFirstSyncedAnimation;
	std::shared_ptr<SyncedAnimation> mSecondSyncedAnimation;
};


class ChessBattleHeadbutt : public ChessBattle {
public:
	ChessBattleHeadbutt(ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	ChessBoardSquare* mSquareSetup;
	bool mIsMovingIntoPosition;
	bool mIsWaitingForTriggeringHeadbut;
	std::shared_ptr<SyncedAnimation> mSyncedAnimation;
};

class ChessBattleStealthKill : public ChessBattle {
public:
	ChessBattleStealthKill();

	void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
};

class ChessBattleDeathByCop : public ChessBattle {
public:
	ChessBattleDeathByCop();

	void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	Any mIncidentId;
};

class ChessBattleJerryCan : public ChessBattle {
public:
	ChessBattleJerryCan();

	void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
};

class ChessBattleHandToHandWeapon : public ChessBattle {
public:
	ChessBattleHandToHandWeapon(std::string handToHandWeaponAttacker, std::string handToHandWeaponDefender, bool defenderIsUnarmed, int defenderHealth);

	void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	std::string mHandToHandWeaponAttacker;
	std::string mHandToHandWeaponDefender;
	bool mDefenderIsUnarmed;
	int mDefenderHealth;
};