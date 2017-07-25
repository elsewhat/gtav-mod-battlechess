#pragma once

#include "..\..\inc\types.h"
#include "ChessBoard.h"
#include <vector>

class EngineModeController {
public:
	virtual void onEnterMode(ChessBoard* chessBoard)=0;
	virtual void onExitMode(ChessBoard* chessBoard)=0 ;
	virtual bool actionOnTick(DWORD tick, ChessBoard* chessBoard)=0;
};
