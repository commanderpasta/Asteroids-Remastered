#include "LevelSystem.h"

LevelSystem::LevelSystem(steady_clock::time_point currentTime) : currentLevel(0), lastTimeShipSpawned(currentTime + seconds(30)), levelChangePause(currentTime), isLevelActive(true) {

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

void LevelSystem::nextLevel(steady_clock::time_point currentTime) {
	this->currentLevel++;
	this->isLevelActive = false;
	this->levelChangePause = currentTime;
}

bool LevelSystem::canShipSpawn(steady_clock::time_point currentTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentTime - this->lastTimeShipSpawned);
	if (timeSpan.count() > 30.0) {
		this->lastTimeShipSpawned = currentTime;
		return true;
	}

	return false;
}

bool LevelSystem::canStartLevel(steady_clock::time_point currentTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentTime - this->levelChangePause);
	if (timeSpan.count() > 2.0) {
		this->isLevelActive = true;
		return true;
	}

	return false;
}