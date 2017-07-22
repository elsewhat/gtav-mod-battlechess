/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#include "gtautils.h"

#include "utils.h"
#include "..\..\inc\natives.h"

#include <windows.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>


const LPCSTR WeaponUtils::weaponNames[] = {
	"WEAPON_KNIFE", "WEAPON_NIGHTSTICK", "WEAPON_HAMMER", "WEAPON_BAT", "WEAPON_GOLFCLUB", "WEAPON_CROWBAR",
	"WEAPON_PISTOL", "WEAPON_COMBATPISTOL", "WEAPON_APPISTOL", "WEAPON_PISTOL50", "WEAPON_MICROSMG", "WEAPON_SMG",
	"WEAPON_ASSAULTSMG", "WEAPON_ASSAULTRIFLE", "WEAPON_CARBINERIFLE", "WEAPON_ADVANCEDRIFLE", "WEAPON_MG",
	"WEAPON_COMBATMG", "WEAPON_PUMPSHOTGUN", "WEAPON_SAWNOFFSHOTGUN", "WEAPON_ASSAULTSHOTGUN", "WEAPON_BULLPUPSHOTGUN",
	"WEAPON_STUNGUN", "WEAPON_SNIPERRIFLE", "WEAPON_HEAVYSNIPER", "WEAPON_GRENADELAUNCHER", "WEAPON_GRENADELAUNCHER_SMOKE",
	"WEAPON_RPG", "WEAPON_MINIGUN", "WEAPON_GRENADE", "WEAPON_STICKYBOMB", "WEAPON_SMOKEGRENADE", "WEAPON_BZGAS",
	"WEAPON_MOLOTOV", "WEAPON_FIREEXTINGUISHER", "WEAPON_PETROLCAN",
	"WEAPON_SNSPISTOL", "WEAPON_SPECIALCARBINE", "WEAPON_HEAVYPISTOL", "WEAPON_BULLPUPRIFLE", "WEAPON_HOMINGLAUNCHER",
	"WEAPON_PROXMINE", "WEAPON_SNOWBALL", "WEAPON_VINTAGEPISTOL", "WEAPON_DAGGER", "WEAPON_FIREWORK", "WEAPON_MUSKET",
	"WEAPON_MARKSMANRIFLE", "WEAPON_HEAVYSHOTGUN", "WEAPON_GUSENBERG", "WEAPON_HATCHET", "WEAPON_RAILGUN",
	"WEAPON_COMBATPDW", "WEAPON_KNUCKLE", "WEAPON_MARKSMANPISTOL"
};

const std::vector<int> GTAModUtils::playerControlsToDisable = {
	0,2,3,4,5,6,16,17,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,44,45,71,72,75,140,141,142,143,156,243,257,261,262,263,264,267,268,269,270,271,272,273
};


void UIUtils::setStatusText(std::string text)
{
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)text.c_str());
	UI::_DRAW_NOTIFICATION(1, 1);
}


void UIUtils::DRAW_TEXT(char* Text, float X, float Y, float S_X, float S_Y, int Font, bool Outline, bool Shadow, bool Center, bool RightJustify, int R, int G, int B, int A)
{
	UI::SET_TEXT_FONT(Font);
	UI::SET_TEXT_SCALE(S_X, S_Y);
	UI::SET_TEXT_COLOUR(R, G, B, A);
	if (Outline)
		UI::SET_TEXT_OUTLINE();
	if (Shadow)
		UI::SET_TEXT_DROP_SHADOW();
	UI::SET_TEXT_CENTRE(Center);
	UI::SET_TEXT_RIGHT_JUSTIFY(RightJustify);
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING(Text);
	UI::_DRAW_TEXT(X, Y);
}

void WeaponUtils::giveAllWeapons(Player playerPed)
{
	for (int i = 0; i < sizeof(WeaponUtils::weaponNames) / sizeof(WeaponUtils::weaponNames[0]); i++) {
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY((char *)WeaponUtils::weaponNames[i]), 1000, 0);
	}
}

void GTAModUtils::checkCorruptPlayerPed()
{
	// common variables
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	if (!ENTITY::DOES_ENTITY_EXIST(playerPed)) return;

	Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
	if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
		if (model != GAMEPLAY::GET_HASH_KEY("player_zero") &&
			model != GAMEPLAY::GET_HASH_KEY("player_one") &&
			model != GAMEPLAY::GET_HASH_KEY("player_two"))
		{
			//UIUtils::setStatusText("turning to normal");
			WAIT(1000);

			model = GAMEPLAY::GET_HASH_KEY("player_zero");
			STREAMING::REQUEST_MODEL(model);
			while (!STREAMING::HAS_MODEL_LOADED(model))
				WAIT(0);
			PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), model);
			PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);

			// wait until player is ressurected
			while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
				WAIT(0);

		}
}

void GTAModUtils::setPedMissionEntity(Ped ped)
{
	if (!ENTITY::DOES_ENTITY_EXIST(ped)) {
		return;
	}

	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(ped, true, true);
	if (PED::IS_PED_IN_ANY_VEHICLE(ped, 0)) {
		Vehicle pedVehicle = PED::GET_VEHICLE_PED_IS_USING(ped);
		ENTITY::SET_ENTITY_AS_MISSION_ENTITY(pedVehicle, true, true);
	}
}

void GTAModUtils::teleportEntityToLocation(Entity entityToTeleport, Vector3 location, bool trustZValue)
{
	Logger::logInfo("teleport_entity_to_location: Entity:" + std::to_string(entityToTeleport));
	//From the native trainer. Could it be replaced with PATHFIND::GET_SAFE_COORD_FOR_PED ?

	if (trustZValue == false) {
		// load needed map region and check height levels for ground existence
		bool groundFound = false;
		static float groundCheckHeight[] = {
			100.0, 150.0, 50.0, 0.0, 200.0, 250.0, 300.0, 350.0, 400.0,
			450.0, 500.0, 550.0, 600.0, 650.0, 700.0, 750.0, 800.0
		};
		for (int i = 0; i < sizeof(groundCheckHeight) / sizeof(float); i++)
		{
			ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entityToTeleport, location.x, location.y, groundCheckHeight[i], 0, 0, 1);
			WAIT(100);
			if (GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(location.x, location.y, groundCheckHeight[i], &location.z, 0))
			{
				groundFound = true;
				location.z += 3.0;
				break;
			}
		}
		// if ground not found then set Z in air and give player a parachute
		if (!groundFound)
		{
			location.z = 1000.0;
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(PLAYER::PLAYER_PED_ID(), 0xFBAB5776, 1, 0);
		}
	}

	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(entityToTeleport, location.x, location.y, location.z, 0, 0, 1);

	//after a teleport, actions some time seems to be stuck
	if (ENTITY::IS_ENTITY_A_PED(entityToTeleport)) {
		//AI::CLEAR_PED_TASKS(entityToTeleport);
	}
	else if (ENTITY::IS_ENTITY_A_VEHICLE(entityToTeleport)) {
		VEHICLE::SET_VEHICLE_ENGINE_ON(entityToTeleport, false, true, false);
		Ped pedDriver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(entityToTeleport, -1);
		if (pedDriver >= 1) {
			//AI::CLEAR_PED_TASKS(pedDriver);
			//AI::TASK_PAUSE(pedDriver, 500);
		}
	}
}

void GTAModUtils::possessPed(Ped swapToPlayerPed)
{
	if (ENTITY::DOES_ENTITY_EXIST(swapToPlayerPed) && ENTITY::IS_ENTITY_A_PED(swapToPlayerPed) && !ENTITY::IS_ENTITY_DEAD(swapToPlayerPed)) {
		Ped swapFromPed = PLAYER::PLAYER_PED_ID();

		if (swapFromPed == swapToPlayerPed) {
			return;
		}


		PLAYER::CHANGE_PLAYER_PED(PLAYER::PLAYER_ID(), swapToPlayerPed, false, false);

		//stop any animations or scenarios being run on the ped
		AI::CLEAR_PED_TASKS(swapToPlayerPed);
	}
	else {
		Logger::logInfo("possess_ped: Could not possess ped (entity may be probably dead) Ped:" + std::to_string(swapToPlayerPed));
	}
}

void GTAModUtils::disableControls()
{
	for (auto & controlCode : playerControlsToDisable) {
		CONTROLS::DISABLE_CONTROL_ACTION(0, controlCode, 1);
	}

	//INPUT_MELEE_ATTACK_LIGHT = 140
	//INPUT_MELEE_ATTACK_HEAVY = 141
	//INPUT_MELEE_ATTACK_ALTERNATE = 142
	//INPUT_MELEE_BLOCK = 143
	//INPUT_VEH_EXIT = 75

	//INPUT_NEXT_CAMERA = 0,
	//INPUT_LOOK_UD = 2,
	//INPUT_LOOK_UP_ONLY = 3,
	//INPUT_LOOK_DOWN_ONLY = 4,
	//INPUT_LOOK_LEFT_ONLY = 5,
	//INPUT_LOOK_RIGHT_ONLY = 6,
	//INPUT_SELECT_NEXT_WEAPON = 16,
	//INPUT_SELECT_PREV_WEAPON = 17,
	//INPUT_CHARACTER_WHEEL = 19,
	//INPUT_MULTIPLAYER_INFO = 20,
	//INPUT_SPRINT = 21,
	//INPUT_JUMP = 22,
	//INPUT_ENTER = 23,
	//INPUT_ATTACK = 24,
	//INPUT_AIM = 25,
	//INPUT_LOOK_BEHIND = 26,
	//INPUT_PHONE = 27,
	//INPUT_SPECIAL_ABILITY = 28,
	//INPUT_SPECIAL_ABILITY_SECONDARY = 29,
	//INPUT_MOVE_LR = 30,
	//INPUT_MOVE_UD = 31,
	//INPUT_MOVE_UP_ONLY = 32,
	//INPUT_MOVE_DOWN_ONLY = 33,
	//INPUT_MOVE_LEFT_ONLY = 34,
	//INPUT_MOVE_RIGHT_ONLY = 35,
	//INPUT_DUCK = 36,
	//INPUT_SELECT_WEAPON = 37,
	//INPUT_COVER = 44,
	//INPUT_RELOAD = 45,
	//INPUT_MAP = 156,
	//INPUT_ENTER_CHEAT_CODE = 243,
	//INPUT_ATTACK2 = 257,
	//INPUT_PREV_WEAPON = 261,
	//INPUT_NEXT_WEAPON = 262,
	//INPUT_MELEE_ATTACK1 = 263,
	//INPUT_MELEE_ATTACK2 = 264,
	//INPUT_MOVE_LEFT = 266,
	//INPUT_MOVE_RIGHT = 267,
	//INPUT_MOVE_UP = 268,
	//INPUT_MOVE_DOWN = 269,
	//INPUT_LOOK_LEFT = 270,
	//INPUT_LOOK_RIGHT = 271,
	//INPUT_LOOK_UP = 272,
	//INPUT_LOOK_DOWN = 273,

}
