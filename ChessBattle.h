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
