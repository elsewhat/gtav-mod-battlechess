#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"

#include "EngineModeController.h"

#include <vector>


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
public:
	BattleChessGameController();
	virtual void onEnterMode() override;
	virtual void onExitMode() override;
	virtual bool actionOnTick(DWORD tick) override;
};
