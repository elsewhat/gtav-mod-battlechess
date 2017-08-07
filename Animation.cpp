#include "Animation.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>


bool Animation::matchesFilter(std::string filterStr){
	std::vector<std::string> filterTokens = StringUtils::split(filterStr, ' ');

	FILTER_OPERATORS currentFitlerOperator = FILTER_OR;
	bool matchesFilter = true;
	int nrMatches = 0;
	for (auto token : filterTokens) {
		if (!token.empty() && (token.compare("AND") == 0 || token.compare("and") == 0)) {
			currentFitlerOperator = FILTER_AND;
			continue;
		}
		else if (!token.empty() && (token.compare("OR") == 0 || token.compare("or") == 0)) {
			currentFitlerOperator = FILTER_OR;
			continue;
		}
		else if (!token.empty() && (token.compare("NOT") == 0 || token.compare("not") == 0)) {
			currentFitlerOperator = FILTER_NOT;
			continue;
		}

		if (currentFitlerOperator == FILTER_AND) {
			if (std::string(animName).find(token) == std::string::npos && std::string(animLibrary).find(token) == std::string::npos) {
				return false;
			}
		}
		else if (currentFitlerOperator == FILTER_NOT) {
			if (std::string(animName).find(token) != std::string::npos || std::string(animLibrary).find(token) != std::string::npos) {
				return false;
			}
		}
		else {//FILTER_OR
			if (std::string(animName).find(token) != std::string::npos || std::string(animLibrary).find(token) != std::string::npos) {
				nrMatches++;
			}
		}
	}

	if (nrMatches > 0) {
		return true;
	}
	else {
		return false;
	}

}





std::vector<Animation> AnimationFactory::getAllAnimations() {
	return mGtaAnimations;
}

bool AnimationFactory::initialize() {
	Logger::logDebug("SyncedAnimationFactory::initialize");
	mGtaAnimations.reserve(99500);


	std::ifstream animationsFile;
	animationsFile.open(mFileName);

	if (!animationsFile){
		return false;
	}
	int index = 1;
	std::string strShortcutIndex;
	std::string animLibrary;
	std::string animName;
	int duration;

	while (animationsFile >> strShortcutIndex >> animLibrary >> animName >> duration)
	{
		mGtaAnimations.push_back({ index,strShortcutIndex,strdup(animLibrary.c_str()),strdup(animName.c_str()),duration });
		index++;
	}

	Logger::logDebug("Loaded " + std::to_string(index) + " animations from " + mFileName);
	return true;
}

Animation AnimationFactory::getAnimationForShortcutIndex(int index) {
	if (index > 0 && index < mGtaAnimations.size()) {
		return mGtaAnimations[index];
	}
	else {
		//0 is the null animation
		return mGtaAnimations[0];
	}
}

Animation AnimationFactory::getAnimationForShortcutIndex(std::string strIndex) {
	strIndex.erase(0, strIndex.find_first_not_of('0'));
	int index = atoi(strIndex.c_str());
	return getAnimationForShortcutIndex(index);
}


AnimationFactory::AnimationFactory(std::string fileName)
{
	mFileName = fileName;
	initialize();
}
