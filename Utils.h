/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include "inc\types.h"

class IOUtils {
public:
	static std::string inputString(int maxLength);
	static DWORD inputDword();
	static float inputFloat();
};

class AssertionException : public std::exception
{
public:
	AssertionException(char* msg);
	virtual const char* what() const throw()
	{
		return whatMsg;
	}
protected:
	char* whatMsg;
};

class Logger {
protected:
	static const bool LogInfo = true;
	static const bool LogDebug = true;
	static const bool LogError = true;
	//value defined in .cpp
	static const char* FileNameLog;
public:
	static void logInfo(std::string msg);
	static void logDebug(std::string msg);
	static void logError(std::string msg);
	static void assert(bool assertion, std::string msg);
private: 
	static void log(std::string logLevel, std::string msg);
	static const std::string currentDateTime();
};




class StringUtils {
public:
	static void split(const std::string &s, char delim, std::vector<std::string> &elems);
	static std::vector<std::string> split(const std::string &s, char delim);
};



class MathUtils {
public:
	static Vector3 rotationToDirection(Vector3 rotation);
	static Vector3 crossProduct(Vector3 a, Vector3 b);
	static std::string roundNumber(float number);
};

#define PI 3.14159265