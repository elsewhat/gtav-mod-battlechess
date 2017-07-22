#include "BattleChessGameController.h"

#include "Keyboard.h"
#include "Utils.h"
#include "GTAUtils.h"
#include <vector>
#include <string>

BattleChessGameController::BattleChessGameController()
{
	Logger::logInfo(" BattleChessGameController constructor");
	shouldExitMode = false;
	cameraSpeedFactor = 0.1;
	camLastPos = {};
}
/**
* Main loop which called from BattleChess
*/
bool BattleChessGameController::actionOnTick(DWORD tick)
{
	GTAModUtils::disableControls();

	/* ACTIONS WHICH MAY REQUIRE A WAIT PERIODE IN TICKS AFTERWAREDS */
	if (nextWaitTicks == 0 || GetTickCount() - mainTickLast >= nextWaitTicks) {
		nextWaitTicks = 0;
		shouldExitMode = false;


		mainTickLast = GetTickCount();

	}

	if (updateCameraMovement()) {
		//we have camera movement. Updated selected recording accordingly

	}

	updateCameraRotation();

	//actions to be used during active scene
	//draw_spot_lights();

	//check if the player is dead/arrested, in order to swap back to original in order to avoid crash
	GTAModUtils::checkCorruptPlayerPed();

	return shouldExitMode;
}

void BattleChessGameController::onEnterMode()
{
	Logger::logInfo(" BattleChessGameController::onEnterMode()");
	scaleForm = GRAPHICS::REQUEST_SCALEFORM_MOVIE("instructional_buttons");

	//CAM::DO_SCREEN_FADE_OUT(1000);

	Cam orgCam = CAM::GET_RENDERING_CAM();

	//Find the location of our camera based on the current actor
	Ped actorPed = PLAYER::PLAYER_PED_ID();
	Vector3 startLocation = ENTITY::GET_ENTITY_COORDS(actorPed, true);
	float startHeading = ENTITY::GET_ENTITY_HEADING(actorPed);
	Vector3 camOffset;
	camOffset.x = (float)sin((startHeading *PI / 180.0f))*10.0f;
	camOffset.y = (float)cos((startHeading *PI / 180.0f))*10.0f;
	camOffset.z = 6.4;

	if (startLocation.x < 0) {
		camOffset.x = -camOffset.x;
	}
	if (startLocation.y < 0) {
		camOffset.y = -camOffset.y;
	}

	Logger::logInfo("actor location (" + std::to_string(startLocation.x) + ", " + std::to_string(startLocation.y) + ", " + std::to_string(startLocation.z) + ")");
	Logger::logInfo("Camera offset (" + std::to_string(camOffset.x) + ", " + std::to_string(camOffset.y) + ", " + std::to_string(camOffset.z) + ")");

	Vector3 camLocation = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(actorPed, camOffset.x, camOffset.y, camOffset.z);
	Logger::logInfo("Camera location (" + std::to_string(camLocation.x) + ", " + std::to_string(camLocation.y) + ", " + std::to_string(camLocation.z) + ")");
	cameraHandle = CAM::CREATE_CAM_WITH_PARAMS("DEFAULT_SCRIPTED_CAMERA", camLocation.x, camLocation.y, camLocation.z, 0.0, 0.0, 0.0, 40.0, 1, 2);

	CAM::POINT_CAM_AT_ENTITY(cameraHandle, actorPed, 0.0f, 0.0f, 0.0f, true);
	WAIT(100);
	CAM::STOP_CAM_POINTING(cameraHandle);

	CAM::RENDER_SCRIPT_CAMS(true, 1, 1800, 1, 0);
}

void BattleChessGameController::onExitMode()
{
	//reset cam
	CAM::RENDER_SCRIPT_CAMS(false, 1, 1500, 1, 0);
	WAIT(1000);
}


void BattleChessGameController::drawInstructions() {
	if (GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(scaleForm)) {
		GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(scaleForm, "CLEAR_ALL");

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "TOGGLE_MOUSE_BUTTONS");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_BOOL(0);
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(scaleForm, "CREATE_CONTAINER");

		char* altControlKey = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 19, 1);
		char* mouseLeftButton = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 330, 1);
		char* mouseRightButton = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 329, 1);
		char* spaceControlKey = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 22, 1);
		char* shiftControlKey = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 21, 1);
		char* ctrlControlKey = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 36, 1);

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("t_D");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("t_A");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("t_S");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("t_W");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Move camera");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(1);
		GRAPHICS::_0xE83A3E3557A56640(shiftControlKey);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Increase camera speed");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(2);
		GRAPHICS::_0xE83A3E3557A56640(ctrlControlKey);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Decrease camera speed");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(3);
		GRAPHICS::_0xE83A3E3557A56640(mouseLeftButton);
		GRAPHICS::_0xE83A3E3557A56640(mouseRightButton);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Camera up/down");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(4);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("t_I");
		if (invertedControls) {
			GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Camera control: Inverted");
		}
		else {
			GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Camera control: Standard");
		}

		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(5);
		GRAPHICS::_0xE83A3E3557A56640(spaceControlKey);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Quick edit position");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "DRAW_INSTRUCTIONAL_BUTTONS");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(-1);
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::DRAW_SCALEFORM_MOVIE_FULLSCREEN(scaleForm, 255, 255, 255, 255,1);
	}
	else {
		Logger::logInfo("Scaleform has not loaded. scaleForm has value " + std::to_string(scaleForm));
	}
}

bool BattleChessGameController::updateCameraRotation()
{

	float rightAxisX = CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, 220);
	float rightAxisY = CONTROLS::GET_DISABLED_CONTROL_NORMAL(0, 221);

	if (rightAxisX != 0.0 || rightAxisY != 0.0) {
		//Rotate camera - Multiply by sensitivity settings
		Vector3 currentRotation = CAM::GET_CAM_ROT(cameraHandle, 2);
		

		if (invertedControls) {
			currentRotation.x += rightAxisY*-5.0f;
		}
		else {
			currentRotation.x += rightAxisY*5.0f;
		}

		currentRotation.z += rightAxisX*-10.0f;
		CAM::SET_CAM_ROT(cameraHandle, currentRotation.x, currentRotation.y, currentRotation.z,2);
		return true;
	}
	else {
		return false;
	}

}


bool BattleChessGameController::updateCameraMovement()
{

	Vector3 camDelta = {};

	bool isMovement = false;
	if (keyPressedCameraForward()) {
		camDelta.x = 1.0;
		isMovement = true;
	}
	if (keyPressedCameraBackward()) {
		camDelta.x = -1.0;
		isMovement = true;
	}
	if (keyPressedCameraLeft()) {
		camDelta.y = -1.0;
		isMovement = true;
	}
	if (keyPressedCameraRight()) {
		camDelta.y = 1.0;
		isMovement = true;
	}
	if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, 329)) {//LMouseBtn
		camDelta.z = 0.3;
		isMovement = true;
	}
	if (CONTROLS::IS_DISABLED_CONTROL_PRESSED(2, 330) ){//RMouseBtn
		camDelta.z = -0.3;
		isMovement = true;
	}
	if (isMovement) {
		if (keyPressedForFastCameraMovement()) {
			camDelta.x *= 3; 
			camDelta.y *= 3;
			camDelta.z *= 3;
		}
		else if (keyPressedForSlowCameraMovement()) {
			camDelta.x /= 20;
			camDelta.y /= 20;
			camDelta.z /= 20;
		}

		camNewPos = CAM::GET_CAM_COORD(cameraHandle);
		camLastPos.x = camNewPos.x;
		camLastPos.y = camNewPos.y;
		camLastPos.z = camNewPos.z;

		Vector3 camRot = {};
		camRot= CAM::GET_CAM_ROT(cameraHandle, 2);
		//camera rotation is not as expected. .x value is rotation in the z-plane (view up/down) and third paramter is the rotation in the x,y plane.

		Vector3 direction = {};
		direction = MathUtils::rotationToDirection(camRot);

		//forward motion
		if (camDelta.x != 0.0) {
			camNewPos.x += direction.x * camDelta.x * cameraSpeedFactor;
			camNewPos.y += direction.y * camDelta.x * cameraSpeedFactor;
			camNewPos.z += direction.z * camDelta.x * cameraSpeedFactor;
		}

		//sideways motion
		if (camDelta.y != 0.0 ) {
			//straight up
			Vector3 b = { };
			b.z = 1.0;

			Vector3 sideWays = {};
			sideWays = MathUtils::crossProduct(direction, b);
				
			camNewPos.x += sideWays.x * camDelta.y * cameraSpeedFactor;
			camNewPos.y += sideWays.y * camDelta.y * cameraSpeedFactor;
		}

		//up/down
		if (camDelta.z != 0.0) {
			camNewPos.z += camDelta.z * cameraSpeedFactor;
		}

		CAM::SET_CAM_COORD(cameraHandle, camNewPos.x, camNewPos.y, camNewPos.z);

		//Logger::logInfo("Cam pos vs last  (" + std::to_string(camNewPos.x) + ", " + std::to_string(camNewPos.y) + ", " + std::to_string(camNewPos.z) + ")(" + std::to_string(camLastPos.x) + ", " + std::to_string(camLastPos.y) + ", " + std::to_string(camLastPos.z) + ")");
		return true;
	}

	return false;

}


bool BattleChessGameController::keyPressedCameraForward() {
	//W
	if (IsKeyDown(0x57)) {
		return true;
	}
	else {
		return false;
	}
}
bool BattleChessGameController::keyPressedCameraBackward() {
	//W
	if (IsKeyDown(0x53)) {
		return true;
	}
	else {
		return false;
	}
}
bool BattleChessGameController::keyPressedCameraLeft() {
	//A
	if (IsKeyDown(0x41)) {
		return true;
	}
	else {
		return false;
	}
}
bool BattleChessGameController::keyPressedCameraRight() {
	//D
	if (IsKeyDown(0x44)) {
		return true;
	}
	else {
		return false;
	}
}

bool BattleChessGameController::keyPressedForFastCameraMovement() {
	//D
	if (IsKeyDown(VK_SHIFT)) {
		return true;
	}
	else {
		return false;
	}
}

bool BattleChessGameController::keyPressedForSlowCameraMovement() {
	//D
	if (IsKeyDown(VK_CONTROL)) {
		return true;
	}
	else {
		return false;
	}
}

bool  BattleChessGameController::keyPressedForInvertedCamera() {
	//D
	if (IsKeyDown(0x49)) {
		return true;
	}
	else {
		return false;
	}
}