#pragma once

#include "inc\types.h"
class ActionGoToSquare;
#include "ChessBattleAction.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
class SyncedAnimation;
#include "SyncedAnimation.h"
#include <vector>
#include <memory>

class ChessBattle {
public:
	ChessBattle(ChessMove chessMove, ChessBoard* chessBoard);

	virtual void initializeBattle(ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard);
	virtual void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)=0;
	virtual bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove,ChessBoard* chessBoard)=0;

protected:
	std::shared_ptr<ActionGoToSquare> mActionGoToEndSquare;
	DWORD mTicksStarted;
	bool mIsMovingToSquare;
	int mNrMovementChecks;
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
	ChessBattleSyncedAnimationChained(ChessMove chessMove, ChessBoard* chessBoard,std::shared_ptr<SyncedAnimation> firstSyncedAnimation, std::shared_ptr<SyncedAnimation> secondSyncedAnimation, bool killAfterwards, bool useDefenderLocation, Vector3 locationOffset);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

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
	ChessBattleHeadbutt(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
	ChessBoardSquare* mSquareSetup;
	bool mIsMovingIntoPosition;
	bool mIsWaitingForTriggeringHeadbut;
	std::shared_ptr<SyncedAnimation> mSyncedAnimation;
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
};

class ChessBattleJerryCan : public ChessBattle {
public:
	ChessBattleJerryCan(ChessMove chessMove, ChessBoard* chessBoard);

	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

protected:
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
