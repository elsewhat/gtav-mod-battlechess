#include "ChessPed.h"

#include "Utils.h"
#include "..\..\inc\natives.h"

ChessPed::ChessPed()
{
	mPedModelHash = 71929310;
}

ChessPed::ChessPed(DWORD pedModelHash, std::array<int, 12> drawableVariations, std::array<int, 12> textureVariation, std::array<int, 12> paletteVariation, std::array<int, 3> propVariation, std::array<int, 3> propTextureVariation)
{
	mPedModelHash = pedModelHash;
	mDrawableVariations = drawableVariations;
	mTextureVariation = textureVariation;
	mPaletteVariation = paletteVariation;
	mPropVariation = propVariation;
	mPropTextureVariation = propTextureVariation;
}


Ped ChessPed::getPed() const
{
	return mPed;
}

void ChessPed::spawnPed(Vector3 location, float heading)
{
	STREAMING::REQUEST_MODEL(mPedModelHash);
	while (!STREAMING::HAS_MODEL_LOADED(mPedModelHash)) {
		WAIT(0);
	}

	float startHeading = ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID());

	mPed = PED::CREATE_PED(4, mPedModelHash, location.x, location.y, location.z, startHeading, false, true);

	Logger::logDebug("Setting ped variant");

	//set variant of ped
	for (int i = 0; i < 12; i++) {
		PED::SET_PED_COMPONENT_VARIATION(mPed, i, mDrawableVariations[i], mTextureVariation[i], mPaletteVariation[i]);
	}

	Logger::logDebug("Setting ped props");
	//get props
	for (int i = 0; i <= 2; i++) {
		PED::SET_PED_PROP_INDEX(mPed, i, mPropVariation[i], mPropTextureVariation[i], 2);
	}

	//clear model from mem
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(mPedModelHash);

}

void ChessPed::revivePed(Vector3 location, float heading)
{
}

void ChessPed::removePed()
{
	if (ENTITY::DOES_ENTITY_EXIST(mPed))
	{
		PED::DELETE_PED(&mPed);
	}
}

bool ChessPed::pedExist() const
{
	if (mPed!= 0 && ENTITY::DOES_ENTITY_EXIST(mPed))
	{
		return true;
	}
	else {
		return false;
	}
}

bool ChessPed::isPedDead() const
{
	if (ENTITY::DOES_ENTITY_EXIST(mPed) && ENTITY::IS_ENTITY_DEAD(mPed)) {
		return true;
	}
	else {
		return false;
	}
}
