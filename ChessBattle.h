#pragma once

#include "inc\types.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include <vector>

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

	void startExecution(DWORD ticksStart, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessMove chessMove, ChessBoard* chessBoard)override;
};
