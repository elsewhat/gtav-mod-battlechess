#pragma once

#include "inc\types.h"
#include <array>

class ChessPed {
public:
	ChessPed();
	ChessPed( DWORD pedModelHash, char* movementStyle, std::array<int, 12> drawableVariations, std::array<int, 12> textureVariation, std::array<int, 12> paletteVariation, std::array<int, 3> propVariation, std::array<int, 3> propTextureVariation);

	Ped getPed() const;

	void spawnPed(Vector3 location, float heading, Hash relationshipGroupHash);
	void revivePed(Vector3 location, float heading);
	void removePed();

	bool pedExist() const;
	bool isPedDeadOrDying() const;
	

protected:
	DWORD mPedModelHash;
	Ped mPed;
	char* mMovementStyle;
	std::array<int, 12> mDrawableVariations;
	std::array<int, 12> mTextureVariation;
	std::array<int, 12> mPaletteVariation;
	std::array<int, 3> mPropVariation;
	std::array<int, 3> mPropTextureVariation;
};


