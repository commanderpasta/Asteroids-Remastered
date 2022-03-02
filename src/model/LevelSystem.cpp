#include "LevelSystem.h"

LevelSystem::LevelSystem() : currentLevel(0), isLevelActive(true) {
}

unsigned int LevelSystem::getAmountOfAsteroidSpawns() {
	if (this->currentLevel == 1) {
		return 4;
	} else if (this->currentLevel == 2) {
		return 6;
	} else {
		return 8;
	}
}

void LevelSystem::nextLevel(steady_clock::time_point currentFrameTime) {
	this->currentLevel++;
	this->isLevelActive = false;
	this->levelChangePause = currentFrameTime;
}

bool LevelSystem::canShipSpawn(steady_clock::time_point currentFrameTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentFrameTime - this->lastTimeShipSpawned);
	if (timeSpan.count() > 30.0) {
		this->lastTimeShipSpawned = currentFrameTime;
		return true;
	}

	return false;
}

int LevelSystem::getCurrentLevel() {
	return this->currentLevel;
}

bool LevelSystem::canStartLevel(steady_clock::time_point currentFrameTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentFrameTime - this->levelChangePause);
	if (timeSpan.count() > 2.0) {
		this->isLevelActive = true;
		return true;
	}

	return false;
}

void LevelSystem::setBeginTime(steady_clock::time_point currentFrameTime) {
	this->lastTimeShipSpawned = currentFrameTime;
	this->lastTimeShipSpawned -= seconds{ 25 };
	this->levelChangePause = currentFrameTime;
}