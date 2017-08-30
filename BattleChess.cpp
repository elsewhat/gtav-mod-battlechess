#pragma once

#include "BattleChess.h"

#include "inc\natives.h"
#include "inc\types.h"

#include "keyboard.h"
#include "utils.h"
#include "gtautils.h"
#include "EngineModeController.h"
#include "Scenario.h"
#include "ClipsetMovement.h"
#include "Lighting.h"
#include "Animation.h"
#include "BattleChessGameController.h"
#include "GTAObject.h"
#include "Tinyxml2.h"
#include "ChessBoard.h"

#include <string>
#include <ctime>
#include <vector>
#include <fstream>
#include <memory>
#include <bitset> 
#include <map>

/*
* Main class for the BattleChess mod
* Responsible for the:
* 1. Initialization
* 2. Main engine loop
* 3. Key attributes shared between different GameModes 
* 4. Menu
*
* The actual actions for each tick of the loop is handled the currently active EngineModeController
*/


ChessBoard* chessBoard;

ChessSetFactory chessSetFactory;

//Key mapping attributes
//These are overwrittein in initReadKeyMapping()
DWORD keyMenu = VK_F7;
DWORD keyMenuUp = VK_NUMPAD8;
DWORD keyMenuDown = VK_NUMPAD2;
DWORD keyMenuLeft = VK_NUMPAD4;
DWORD keyMenuRight = VK_NUMPAD6;
DWORD keyMenuSelect = VK_NUMPAD5;
DWORD keyMenuDelete = VK_DELETE;
DWORD keyMenuBack = VK_NUMPAD0;
char keyMenuStr[256];

GAME_MODE gameMode = GAME_MODE_MENU;
std::shared_ptr<EngineModeController> engineModeController = std::make_shared<BattleChessGameController>(BattleChessGameController());

bool doDisplayMenu = false;

//if menuActiveIndex ==-1 , then menu_active_ped is used to show selected index
int menuActiveIndex = 0;
MENU_ITEM menuActiveAction = MENU_ITEM_START_GAME;
int menuMaxIndex = 0;

bool submenuIsDisplayed = false;
bool submenuIsActive = false;
int submenuActiveIndex = -1;
int submenuMaxIndex = 0;
MENU_ITEM submenuActiveAction = MENU_ITEM_START_GAME;

//Used for determining wait time after actions
DWORD mainTickLast=0;
DWORD nextWaitTicks = 0;

//scaleform instructional_buttons reference
int scaleForm;

//Config files
std::string configFileLocations = "Battlechess_locations.xml";
std::string configFileSets= "Battlechess_sets.xml";
std::string configFileAnimations = "Battlechess_anims.txt";
std::string configFileSynchedAnimations = "Battlechess_synchedanims.xml";
LPCSTR configFileIni = ".\\battlechess.ini";

void showVersionInfo(){
	UIUtils::setStatusText("Battle Chess 0.0.1 by elsewhat");
}


bool shouldDisplayMenu() {
	return doDisplayMenu;
}

bool keyPressedMenuToggle()
{
	return IsKeyJustUp(keyMenu);
}


void drawInstructionalButtons() {
	if (GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(scaleForm)) {
		GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(scaleForm, "CLEAR_ALL");

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "TOGGLE_MOUSE_BUTTONS");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_BOOL(0);
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::CALL_SCALEFORM_MOVIE_METHOD(scaleForm, "CREATE_CONTAINER");

		char* altControlKey = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 19, 1);
		char* spaceControlKey = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 22, 1);
		char* delControlKey = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 178, 1);
		char* insControlKey = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 121, 1);
		char* endControlKey = CONTROLS::_GET_CONTROL_ACTION_NAME(2, 179, 1);

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(0);
		std::string scaleFormKey = "t_" + std::string(keyMenuStr);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING(strdup(scaleFormKey.c_str()));
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Hide HUD menu");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(1);
		GRAPHICS::_0xE83A3E3557A56640(delControlKey);
		GRAPHICS::_0xE83A3E3557A56640(altControlKey);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Back to start");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();

		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(2);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("t_F");
		GRAPHICS::_0xE83A3E3557A56640(altControlKey);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Enter as passenger");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();


		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(3);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("END Explode nearby veh");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();


		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(4);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("t_A");
		GRAPHICS::_0xE83A3E3557A56640(altControlKey);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Autopilot");
		GRAPHICS::_POP_SCALEFORM_MOVIE_FUNCTION_VOID();


		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION(scaleForm, "SET_DATA_SLOT");
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_INT(5);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("t_C");
		GRAPHICS::_0xE83A3E3557A56640(altControlKey);
		GRAPHICS::_PUSH_SCALEFORM_MOVIE_FUNCTION_PARAMETER_STRING("Firing squad");
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

void drawSubmenuAnimation(int drawIndex) {
	int submenu_index = 0;

	//colors for swapping from active to inactive... messy
	int textColorR = 255, textColorG = 255, textColorB = 255;
	int bgColorR = 0, bgColorG = 0, bgColorB = 0;


	if (submenuIsActive && submenuActiveIndex == submenu_index) {
		textColorR = 0, textColorG = 0, textColorB = 0, bgColorR = 255, bgColorG = 255, bgColorB = 255;
		submenuActiveAction = SUBMENU_ITEM_GAME_TYPE;
	}
	else {
		textColorR = 255, textColorG = 255, textColorB = 255, bgColorR = 0, bgColorG = 0, bgColorB = 0;
	}

	UIUtils::DRAW_TEXT("Game type", 0.76, 0.888 - (0.04)*drawIndex, 0.3, 0.3, 0, false, false, false, false, textColorR, textColorG, textColorB, 200);
	GRAPHICS::DRAW_RECT(0.81, 0.900 - (0.04)*drawIndex, 0.113, 0.034, bgColorR, bgColorG, bgColorB, 100);

	drawIndex++;
	submenu_index++;


	submenuMaxIndex = submenu_index;

}

void drawMainMenu() {
	int drawIndex = 0;
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	submenuIsDisplayed = false;

	//colors for swapping from active to inactive... messy
	int textColorR = 255, textColorG = 255, textColorB=255;
	int bgColorR = 0, bgColorG = 0, bgColorB = 0;
	if (menuActiveIndex == drawIndex) {
		textColorR = 0, textColorG = 0, textColorB = 0, bgColorR = 255, bgColorG = 255, bgColorB = 255;
	} else {
		textColorR = 255, textColorG = 255, textColorB = 255, bgColorR = 0, bgColorG = 0, bgColorB = 0;
	}

	UIUtils::DRAW_TEXT("BETA RELEASE OF BATTLECHESS BY ELSEWHAT - NOT FOR DISTRIBUTION", 0.0, 0.0, 0.3, 0.3, 0, false, false, false, false, 255, 255, 255, 155);

	float heading= ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());
	Vector3 location = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
	UIUtils::DRAW_TEXT(strdup(("Location (" + std::to_string(location.x) + "," + std::to_string(location.y) + "," + std::to_string(location.z) + ") Heading: "+std::to_string(heading)).c_str()) , 0.0, 0.2, 0.3, 0.3, 0, false, false, false, false, 255, 255, 255, 155);

	UIUtils::DRAW_TEXT("Start game", 0.88, 0.888 - (0.04)*drawIndex, 0.3, 0.3, 0, false, false, false, false, textColorR, textColorG, textColorB, 200);
	GRAPHICS::DRAW_RECT(0.93, 0.900 - (0.04)*drawIndex, 0.113, 0.034, bgColorR, bgColorG, bgColorB, 100);


	if (menuActiveIndex == drawIndex) {
		menuActiveAction = MENU_ITEM_START_GAME;
	}
	drawIndex++;

	if (menuActiveIndex == -1) {
		menuActiveIndex = 0;
	}

	menuMaxIndex = drawIndex - 1;
	if (menuActiveIndex > menuMaxIndex) {
		menuActiveIndex = menuMaxIndex;
	}

	if (submenuIsDisplayed == false) {
		submenuIsActive = false;
		submenuActiveIndex = -1;
	}

}


void initReadKeyMapping() {
	GetPrivateProfileString("keys", "keyMenu", "F7", keyMenuStr, sizeof(keyMenuStr), configFileIni);

	Logger::logInfo("Read keys from ini file keyMenu " + std::string(keyMenuStr));

	keyMenu = str2key(std::string(keyMenuStr));
	if (keyMenu == 0) {
		Logger::logInfo(std::string(keyMenuStr) + " is not a valid key");
		keyMenu = str2key("F10");
	}

	Logger::logInfo("Converted keys to dword key_possess " + std::to_string(keyMenu));

	char key_menu_down_str[256], key_menu_up_str[256], key_menu_select_str[256], key_menu_left_str[256], key_menu_right_str[256], key_menu_delete_str[256];
	GetPrivateProfileString("keys", "keyMenuDown", "NUM2", key_menu_down_str, sizeof(key_menu_down_str), configFileIni);
	GetPrivateProfileString("keys", "keyMenuUp", "NUM8", key_menu_up_str, sizeof(key_menu_up_str), configFileIni);
	GetPrivateProfileString("keys", "keyMenuSelect", "NUM5", key_menu_select_str, sizeof(key_menu_select_str), configFileIni);
	GetPrivateProfileString("keys", "keyMenuLeft", "NUM4", key_menu_left_str, sizeof(key_menu_left_str), configFileIni);
	GetPrivateProfileString("keys", "keyMenuRight", "NUM6", key_menu_right_str, sizeof(key_menu_right_str), configFileIni);
	GetPrivateProfileString("keys", "keyMenuDelete", "DELETE", key_menu_delete_str, sizeof(key_menu_delete_str), configFileIni);


	keyMenuDown = str2key(std::string(key_menu_down_str));
	if (keyMenuDown == 0) {
		Logger::logInfo(std::string(key_menu_down_str) + " is not a valid key");
		keyMenuDown = str2key("NUM2");
	}

	keyMenuUp = str2key(std::string(key_menu_up_str));
	if (keyMenuUp == 0) {
		Logger::logInfo(std::string(key_menu_up_str) + " is not a valid key");
		keyMenuUp = str2key("NUM8");
	}

	keyMenuSelect = str2key(std::string(key_menu_select_str));
	if (keyMenuSelect == 0) {
		Logger::logInfo(std::string(key_menu_select_str) + " is not a valid key");
		keyMenuSelect = str2key("NUM5");
	}

	keyMenuLeft = str2key(std::string(key_menu_left_str));
	if (keyMenuLeft == 0) {
		Logger::logInfo(std::string(key_menu_left_str) + " is not a valid key");
		keyMenuLeft = str2key("NUM4");
	}

	keyMenuRight = str2key(std::string(key_menu_right_str));
	if (keyMenuRight == 0) {
		Logger::logInfo(std::string(key_menu_right_str) + " is not a valid key");
		keyMenuRight = str2key("NUM6");
	}

	keyMenuDelete = str2key(std::string(key_menu_delete_str));
	if (keyMenuDelete == 0) {
		Logger::logInfo(std::string(key_menu_delete_str) + " is not a valid key");
		keyMenuDelete = str2key("DELETE");
	}

}

void actionSubmenuActiveSelected() {
	Logger::logInfo("actionSubmenuActiveSelected " + std::to_string(submenuActiveAction));
	if (submenuActiveAction == SUBMENU_ITEM_GAME_TYPE) {
		//
	}
}

void actionMenuActiveSelected() {
	Logger::logInfo("actionMenuActiveSelected " + std::to_string(menuActiveAction));

	 if (menuActiveAction == MENU_ITEM_START_GAME) {
		 gameMode = GAME_MODE_GAME;
		 nextWaitTicks = 200;
		 Logger::logInfo("BattleChessGameController before creating");
		 engineModeController = std::make_shared<BattleChessGameController>(BattleChessGameController());
		 chessBoard->spawnChessPieces();
		 Logger::logInfo("After chessBoard->spawnChessPieces");

		 engineModeController->onEnterMode(chessBoard);
	}
}

bool keyPressedMenuUp() {
	if (IsKeyDown(keyMenuUp)) {
		return true;
	}
	else {
		return false;
	}
}

bool keyPressedMenuDown() {
	if (IsKeyDown(keyMenuDown)) {
		return true;
	}
	else {
		return false;
	}
}

bool keyPressedMenuLeft() {
	if (IsKeyDown(keyMenuLeft)) {
		return true;
	}
	else {
		return false;
	}
}

bool keyPressedMenuRight() {
	if (IsKeyDown(keyMenuRight)) {
		return true;
	}
	else {
		return false;
	}
}


bool keyPressedMenuSelect() {
	if (IsKeyDown(keyMenuSelect)) {
		return true;
	}
	else {
		return false;
	}
}

bool keyPressedMenuDelete() {
	if (IsKeyDown(keyMenuDelete)) {
		return true;
	}
	else {
		return false;
	}
}

bool keyPressedMenuBack() {
	if (IsKeyDown(keyMenuBack)) {
		return true;
	}
	else {
		return false;
	}
}

void menuActionUp() {
	if (submenuIsActive == false) {
		menuActiveIndex++;
		if (menuActiveIndex > menuMaxIndex) {
			menuActiveIndex = 0;
			nextWaitTicks = 200;
		}
		else {
			submenuIsDisplayed = false;
			submenuIsActive = false;
			submenuActiveIndex = -1;
		}
	}
	else {
		submenuActiveIndex++;
		if (submenuActiveIndex > submenuMaxIndex) {
			submenuActiveIndex = submenuMaxIndex;
		}
	}
	nextWaitTicks = 100;
}

void menuActionDown() {
	if (submenuIsActive == false) {
		menuActiveIndex--;
		if (menuActiveIndex < 0) {
			menuActiveIndex = menuMaxIndex;
			nextWaitTicks = 200;
		}
		else {
			submenuIsDisplayed = false;
			submenuIsActive = false;
			submenuActiveIndex = -1;
		}
	}
	else {
		submenuActiveIndex--;
		if (submenuActiveIndex < 0) {
			submenuActiveIndex = 0;
		}
	}
	nextWaitTicks = 100;
}

void menuActionLeft() {
	if (submenuIsDisplayed) {
		submenuIsActive = true;
		submenuActiveIndex = 0;
	}
}

void menuActionRight() {
	if (submenuIsDisplayed) {
		submenuIsActive = false;
	}
}

void menuActionSelect() {
	if (submenuIsActive) {
		actionSubmenuActiveSelected();
	}
	else {
		actionMenuActiveSelected();
	}
	nextWaitTicks = 200;
}

void mainEngineLoop()
{
	while (true)
	{
		if (gameMode == GAME_MODE_MENU) {
			/* ACTIONS WHICH MAY NEED TO WAIT A FEW TICKS */
			if (nextWaitTicks == 0 || GetTickCount() - mainTickLast >= nextWaitTicks) {
				//nextWaitTicks will be set by action methods in order to define how long before next input can be processed
				nextWaitTicks = 0;

				if (keyPressedMenuToggle()) {
					if (doDisplayMenu) {
						doDisplayMenu = false;
					}
					else {
						showVersionInfo();
						doDisplayMenu = true;
					}
				}

				if (shouldDisplayMenu()) {
					if (keyPressedMenuUp()) {
						menuActionUp();
					}
					else if (keyPressedMenuDown()) {
						menuActionDown();
					}
					else if (keyPressedMenuLeft()) {
						menuActionLeft();
					}
					else if (keyPressedMenuRight()) {
						menuActionRight();
					}
					else if (keyPressedMenuSelect()) {
						menuActionSelect();
					}
				}

				mainTickLast = GetTickCount();
			}

			/* ACTIONS WHICH ARE PERFORMED EVERY TICK */
			//Display HUD for app
			if (shouldDisplayMenu()) {
				drawInstructionalButtons();
				drawMainMenu();
				//disable ALT key
				CONTROLS::DISABLE_CONTROL_ACTION(0, 19, 1);
			}
			else {
				CONTROLS::ENABLE_CONTROL_ACTION(0, 19, 1);
			}

			//check if the player is dead/arrested, in order to swap back to original in order to avoid crash
			GTAModUtils::checkCorruptPlayerPed();

			//Wait for next tick
			WAIT(0);
		}
		else {
			engineModeController->actionOnTick(GetTickCount(), chessBoard);
			WAIT(0);
		}
	}
}

void BattleChessMain()
{
	srand(time(NULL));
	GRAPHICS::REQUEST_STREAMED_TEXTURE_DICT("CommonMenu", 0);
	scaleForm = GRAPHICS::REQUEST_SCALEFORM_MOVIE_INSTANCE("instructional_buttons");
	
	Logger::logInfo("Waiting for instructional_buttons to load");
	while (!GRAPHICS::HAS_SCALEFORM_MOVIE_LOADED(scaleForm)) {
		WAIT(0);
	}
	Logger::logInfo("instructional_buttons have loaded");

	showVersionInfo();

	initReadKeyMapping();


	//initializeSyncedAnimations();

	//Chess board initalization
	chessSetFactory = ChessSetFactory();

	Vector3 baseLocation;
	float baseHeading = 0.0f;
	//air strip
	baseLocation.x = 1629.0;
	baseLocation.y = 3215.0;
	baseLocation.z = 39.55;

	//Racing track
	//1111.885, 106.282, 80.776sd
	//Funfair
	baseLocation.x = -1570.098;
	baseLocation.y = -1122.152;
	baseLocation.z = 3.403;


	//Rockford hills
	baseLocation.x = -830.41;
	baseLocation.y = -123.37;
	baseLocation.z = 37.55;
	baseHeading = 27.5f;


	float squareDeltaX = 2.0; 
	float squareDeltaY = 2.0;
	chessBoard = new ChessBoard(baseLocation, baseHeading, squareDeltaX, squareDeltaY);

	chessBoard->setWhiteChessSet(chessSetFactory.getDefaultWhiteChessSet());
	chessBoard->setBlackChessSet(chessSetFactory.getDefaultBlackChessSet());

	Logger::logInfo("BattleChess initialized");

	AnimationFactory* animationFactory = new AnimationFactory(configFileAnimations);
	SyncedAnimationFactory* syncedAnimationFactory = new SyncedAnimationFactory(configFileSynchedAnimations, animationFactory);

	chessBoard->setSyncedAnimationFactory(syncedAnimationFactory);
	chessBoard->setAnimationFactory(animationFactory);

	mainEngineLoop();
}