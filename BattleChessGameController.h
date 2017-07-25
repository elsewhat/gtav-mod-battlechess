#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"

#include "EngineModeController.h"

#include <vector>
#include <memory>


class BattleChessGameController : public EngineModeController {
private:
	Any cameraHandle;
	int scaleForm;
	float cameraSpeedFactor = 0.1;
	bool shouldExitMode = false;
	bool invertedControls = true;

	DWORD nextWaitTicks = 0;
	DWORD mainTickLast = 0;
	Vector3 camLastPos;
	Vector3 camNewPos;

	ChessBoardSquare* selectedBoardSquare = NULL;
	ChessBoardSquare* cursorBoardSquare = NULL;

	void drawInstructions();

	bool updateCameraRotation();
	bool updateCameraMovement();

	bool keyPressedCameraForward();
	bool keyPressedCameraBackward();
	bool keyPressedCameraLeft();
	bool keyPressedCameraRight();
	bool keyPressedForFastCameraMovement();
	bool keyPressedForSlowCameraMovement();
	bool keyPressedForInvertedCamera();

	bool updateBoardCursorMovement(ChessBoard* chessBoard);

	bool keyPressedBoardUp();
	bool keyPressedBoardUpLeft();
	bool keyPressedBoardUpRight();
	bool keyPressedBoardLeft();
	bool keyPressedBoardRight();
	bool keyPressedBoardSelect();
	bool keyPressedBoardDown();
	bool keyPressedBoardDownLeft();
	bool keyPressedBoardDownRight();

public:
	BattleChessGameController();
	virtual void onEnterMode(ChessBoard* chessBoard) override;
	virtual void onExitMode(ChessBoard* chessBoard) override;
	virtual bool actionOnTick(DWORD tick, ChessBoard* chessBoard) override;
};
