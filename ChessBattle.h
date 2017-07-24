#pragma once

#include "..\..\inc\types.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include <vector>

class ChessBattle {
public:
	virtual bool canBeExecutedFor(const ChessPiece attacker, const ChessPiece defender)=0;
	virtual void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, const ChessBoardSquare squareFrom, const ChessBoardSquare squareTo)=0;
	virtual bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, const ChessBoardSquare squareFrom, const ChessBoardSquare squareTo)=0;
};

class ChessBattleFirePrimaryWeapon : public ChessBattle {
public:
	ChessBattleFirePrimaryWeapon();

	bool canBeExecutedFor(ChessPiece attacker, ChessPiece defender)override;
	void startExecution(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender,const ChessBoardSquare squareFrom, const ChessBoardSquare squareTo)override;
	bool isExecutionCompleted(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, const ChessBoardSquare squareFrom, const ChessBoardSquare squareTo)override;
};
