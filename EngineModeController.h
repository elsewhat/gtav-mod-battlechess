#pragma once

#include "..\..\inc\types.h"
#include <vector>

class EngineModeController {
public:
	virtual void onEnterMode()=0;
	virtual void onExitMode()=0 ;
	virtual bool actionOnTick(DWORD tick)=0;
	//TODO: Include pointer params to key attributes
};
