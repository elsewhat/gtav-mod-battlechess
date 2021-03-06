/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "inc\types.h"
class Animation;
#include "Animation.h"


class GTAModUtils {
protected:
	static const std::vector<int> playerControlsToDisable;
public:
	static void checkCorruptPlayerPed();
	static void setPedMissionEntity(Ped ped);
	static void teleportEntityToLocation(Entity entityToTeleport, Vector3 location, bool trustZValue);
	static void possessPed(Ped swapToPlayerPed);
	static void disableControls();
	static void playAnimation(Ped ped, Animation animation);
	static void enableRadio(bool activate);
	static void setRadioStation(std::string radioStation);
};

class WeaponUtils {
protected:
	static char* weaponNames[];
	static std::vector<std::string> handToHandWeaponNames;
public:
	static std::string getRandomHandToHandWeaponName();
	static void giveAllWeapons(Ped ped); 
	static void giveWeapon(Ped ped, std::string weaponName);
};


class UIUtils {
public:
	static void setStatusText(std::string text);
	static void UIUtils::DRAW_TEXT(char* Text, float X, float Y, float S_X, float S_Y, int Font, bool Outline, bool Shadow, bool Center, bool RightJustify, int R, int G, int B, int A);
};
