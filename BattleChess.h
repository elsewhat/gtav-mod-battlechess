/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"
#include "inc\main.h"
#include "Animation.h"
#include <string>

enum GAME_MODE {
	GAME_MODE_MENU,
	GAME_MODE_GAME
};


enum MENU_ITEM {
	MENU_ITEM_START_GAME = 1,
	SUBMENU_ITEM_GAME_TYPE= 1000,
};

void BattleChessMain();

//TODO: Class-ify all the things!
void avoid_corrupt_playerped();

bool keyPressedMenuUp();
bool keyPressedMenuDown();
bool keyPressedMenuLeft();
bool keyPressedMenuRight();
bool keyPressedMenuSelect();
bool keyPressedMenuDelete();
bool keyPressedMenuBack();

void menuActionUp();
void menuActionDown();
void menuActionLeft();
void menuActionRight();
void menuActionSelect();
void menuActionDelete();

bool shouldDisplayMenu();