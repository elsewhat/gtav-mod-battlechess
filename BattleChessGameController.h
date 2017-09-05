#pragma once

#include "inc\natives.h"
#include "inc\types.h"

#include "EngineModeController.h"
#include "ChessMove.h"

#include <vector>
#include <memory>


class BattleChessGameController : public EngineModeController {
private:
	Any cameraHandle;
	int scaleForm;
	float cameraSpeedFactor = 0.1;
	bool shouldExitMode = false;
	bool invertedControls = true;
	bool moveCameraMode = false;
	bool hasCameraMoveSinceSquarePos = false;

	DWORD nextWaitTicks = 0;
	DWORD mainTickLast = 0;
	Vector3 camLastPos;
	Vector3 camNewPos;

	Vector3 mousePos;

	ChessSide::Side sideToMove;

	ChessBoardSquare* selectedBoardSquare = NULL;
	ChessBoardSquare* cursorBoardSquare = NULL;
	std::vector<ChessMove> possibleMoves;

	ChessMove currentChessMove;
	std::shared_ptr<ChessBattle> currentChessBattle=nullptr;

	void drawInstructions();

	bool updateCameraRotation();
	bool updateCameraMovement();

	bool updateMouseMovement(ChessBoard* chessBoard);
	bool updateMouseSelect(ChessBoard* chessBoard);

	bool keyPressedCameraForward();
	bool keyPressedCameraBackward();
	bool keyPressedCameraLeft();
	bool keyPressedCameraRight();
	bool keyPressedForFastCameraMovement();
	bool keyPressedForSlowCameraMovement();
	bool keyPressedForInvertedCamera();
	bool keyPressedForToggleCameraMode();

	bool updateBoardCursorMovement(ChessBoard* chessBoard);
	bool updateBoardSelect(ChessBoard* chessBoard);

	bool keyPressedBoardUp();
	bool keyPressedBoardUpLeft();
	bool keyPressedBoardUpRight();
	bool keyPressedBoardLeft();
	bool keyPressedBoardRight();
	bool keyPressedBoardSelect();
	bool keyPressedBoardDown();
	bool keyPressedBoardDownLeft();
	bool keyPressedBoardDownRight();

	void resetPossibleMoves();
	void highlightPossibleMoves();

	ChessMove findPossibleMoveFromTo(ChessBoardSquare* squareFrom, ChessBoardSquare* squareTo);

public:
	BattleChessGameController();
	virtual void onEnterMode(ChessBoard* chessBoard) override;
	virtual void onExitMode(ChessBoard* chessBoard) override;
	virtual bool actionOnTick(DWORD tick, ChessBoard* chessBoard) override;
};
