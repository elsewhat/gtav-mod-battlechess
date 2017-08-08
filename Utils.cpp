/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#include "Utils.h"

#include <windows.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "BattleChess.h"


const char* Logger::FileNameLog = "BattleChess.log";

std::string MathUtils::roundNumber(float number)
{
	std::ostringstream out;
	out << std::setprecision(1) << std::fixed << std::showpoint << number;
	std::string roundResult = out.str();
	return roundResult;
}

bool MathUtils::percentageChance(int percentage)
{
	//adapted from https://stackoverflow.com/questions/12657962/how-do-i-generate-a-random-number-between-two-variables-that-i-have-stored
	int min = 1;
	int max = 100;

	int n = max - min + 1;
	int remainder = RAND_MAX % n;
	int x;
	do {
		x = rand();
	} while (x >= RAND_MAX - remainder);

	Logger::logDebug("Change: " + std::to_string((min + x % n)) + "%");
	if ((min + x % n) <= percentage) {
		return true;
	}
	else {
		return false;
	}
}

float IOUtils::inputFloat()
{
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "FMMC_KEY_TIP8", "", "", "", "", "", 6);

	while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) {
		WAIT(0);
	}


	if (GAMEPLAY::IS_STRING_NULL_OR_EMPTY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())) {
		Logger::logInfo("Got null keyboard value");
		return -1.0f;
	}
	char * keyboardValue = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
	std::string strValue = std::string(keyboardValue);
	Logger::logInfo("Got keyboard value " + strValue);
	return atof(keyboardValue);
}

DWORD IOUtils::inputDword()
{
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "FMMC_KEY_TIP8", "", "", "", "", "", 6);

	while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) {
		WAIT(0);
	}


	if (GAMEPLAY::IS_STRING_NULL_OR_EMPTY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())) {
		Logger::logInfo("Got null keyboard value");
		return 0;
	}
	char * keyboardValue = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
	std::string strValue = std::string(keyboardValue);
	Logger::logInfo("Got keyboard value " + strValue);
	return atoi(keyboardValue);
}

std::string IOUtils::inputString(int maxLength)
{
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(true, "FMMC_KEY_TIP8", "", "", "", "", "", maxLength);

	while (GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD() == 0) {
		WAIT(0);
	}


	if (GAMEPLAY::IS_STRING_NULL_OR_EMPTY(GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT())) {
		Logger::logInfo("Got null keyboard value");
		return std::string();
	}
	char * keyboardValue = GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
	std::string strValue = std::string(keyboardValue);
	Logger::logInfo("Got keyboard value " + strValue);
	return strValue;
}


Vector3 MathUtils::rotationToDirection(Vector3 rotation)
{
	//big thanks to camxxcore's C# code https://github.com/CamxxCore/ScriptCamTool/blob/master/GTAV_ScriptCamTool/Utils.cs
	float retZ = rotation.z * 0.01745329f;
	float retX = rotation.x * 0.01745329f;
	float absX = abs(cos(retX));
	Vector3 retVector = { 0.0,0.0,0.0 };
	retVector.x = (float)-(sin(retZ) * absX);
	retVector.y = (float)cos(retZ) * absX;
	retVector.z = (float)sin(retX);
	return retVector;
}

Vector3 MathUtils::crossProduct(Vector3 a, Vector3 b)
{
	//http://onlinemschool.com/math/assistance/vector/multiply1/
	Vector3 retVector = { 0.0,0.0,0.0 };
	retVector.x = a.y*b.z - a.z*b.y;
	retVector.y = a.z*b.x - a.x*b.z;
	retVector.z = a.x*b.y - a.y*b.x;
	return retVector;
}


void StringUtils::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


std::vector<std::string> StringUtils::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

void Logger::logInfo(std::string msg)
{
	if (Logger::LogInfo) {
		log("Info", msg);
	}
}

void Logger::logDebug(std::string msg)
{
	if (Logger::LogDebug) {
		log("Debug", msg);
	}
}

void Logger::logError(std::string msg)
{
	if (Logger::LogError) {
		log("Error", msg);
	}
}

void Logger::assert(bool assertion, std::string msg)
{
	if (!assertion) {
		log("ASSERT", msg);

		//Bad and lazy conversion to char*
		throw AssertionException(strdup(msg.c_str()));
	}
}

void Logger::log(std::string logLevel, std::string msg)
{
	std::ofstream logfile;
	logfile.open(Logger::FileNameLog, std::ios_base::app);

	logfile << msg + "\n";


	logfile.close();
}

const std::string Logger::currentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

	return buf;
}

AssertionException::AssertionException(char * msg)
{
	whatMsg = msg;
}
