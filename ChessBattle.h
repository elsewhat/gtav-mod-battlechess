#pragma once

#include "inc\types.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include <vector>

class ChessBattle {
public:
	ChessBattle();

	virtual bool canBeExecutedFor(ChessMove chessMove)=0;
	virtual void startExecution(DWORD ticksStart, ChessMove chessMove)=0;
	virtual bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove)=0;

protected:
	DWORD mTicksStarted;
};

class ChessBattleFirePrimaryWeapon : public ChessBattle {
public:
	ChessBattleFirePrimaryWeapon();

	bool canBeExecutedFor(ChessMove chessMove)override;
	void startExecution(DWORD ticksStart, ChessMove chessMove)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove)override;
};
