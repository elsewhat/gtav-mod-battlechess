#pragma once

#include "inc\types.h"
class ChessBoardSquare;
#include "ChessBoard.h"
#include "ChessPiece.h"
class SyncedAnimation;
#include "SyncedAnimation.h"
#include <vector>
#include <memory>

/*
Reusable actions for ChessBattles
*/
class ChessBattleAction {
public:
	ChessBattleAction();

	virtual void start(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard) = 0;
	virtual bool hasBeenStarted();
	virtual bool checkForCompletion(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard) = 0;


protected:
	DWORD mTicksStarted;
	bool mHasBeenStarted;
	bool mIsCompleted;
};

class ActionGoToCoord : public ChessBattleAction {
public:
	ActionGoToCoord(Vector3 location, float heading, float walkingSpeed);

	void start(DWORD ticksStart, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;
	bool checkForCompletion(DWORD ticksNow, ChessPiece* attacker, ChessPiece* defender, ChessMove chessMove, ChessBoard* chessBoard)override;

	void setMinDistance(float minDistance);

protected:
	Vector3 mLocation;
	float mHeading;
	float mWalkingSpeed;
	float mMinDistance;
};

class ActionGoToSquare : public ActionGoToCoord
{
public:
	ActionGoToSquare(ChessBoardSquare* squareTo, float heading, float walkingSpeed);
protected:
};
