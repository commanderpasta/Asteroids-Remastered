#include "LevelSystem.h"

/**
 * Creates a level system to manage level states.
 */
LevelSystem::LevelSystem() : currentLevel(0), isLevelActive(true) {
}

/**
 * Decides the amount of asteroids that should spawn in this level.
 * 
 * Spawns more asteroids in the later levels for added complexity.
 * 
 * \return The amount of asteroids that should be spawned.
 */
unsigned int LevelSystem::getAmountOfAsteroidSpawns() {
	if (this->currentLevel == 1) {
		return 4;
	} else if (this->currentLevel == 2) {
		return 6;
	} else {
		return 8;
	}
}

/**
 * Changes to the next level.
 * 
 * \param currentTickTime The time of the current game tick.
 */
void LevelSystem::nextLevel(steady_clock::time_point currentTickTime) {
	this->currentLevel++;
	this->isLevelActive = false;
	this->levelChangePause = currentTickTime;
}

/**
 * Decides whether a ship should spawn.
 * 
 * Ships spawning have a cooldown of 30 seconds.
 * 
 * \param currentTickTime The time of the current game tick.
 * \return A bool that says whether the ship should spawn.
 */
bool LevelSystem::canShipSpawn(steady_clock::time_point currentTickTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentTickTime - this->lastTimeShipSpawned);
	if (timeSpan.count() > 30.0) {
		this->lastTimeShipSpawned = currentTickTime;
		return true;
	}

	return false;
}

/**
 * Get the current level.
 * 
 * \return The current level number.
 */
int LevelSystem::getCurrentLevel() {
	return this->currentLevel;
}

/**
 * Decides whether the level is ready to begin after a level change pause.
 * 
 * The level change pause lasts two seconds.
 * 
 * \param currentTickTime The time of the current game tick.
 * \return A bool that says whether the level can start.
 */
bool LevelSystem::canStartLevel(steady_clock::time_point currentTickTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentTickTime - this->levelChangePause);
	if (timeSpan.count() > 2.0) {
		this->isLevelActive = true;
		return true;
	}

	return false;
}

/**
 * Sets the first time a ship can spawn based on the first tick's time.
 * 
 * \param currentTickTime The time of the current game tick.
 */
void LevelSystem::setBeginTime(steady_clock::time_point currentTickTime) {
	this->lastTimeShipSpawned = currentTickTime;
	this->lastTimeShipSpawned -= seconds{ 25 };
	this->levelChangePause = currentTickTime;
}