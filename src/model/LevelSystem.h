#pragma once

#include <chrono>
using namespace std::chrono;

/**
 * Manages the level system of the game.
 *
 * Uses the current time to decide on level system logic like respawns, difficulty and level starts.
 */
class LevelSystem {
	unsigned int currentLevel; /**< The current level of the game. */
	steady_clock::time_point lastTimeShipSpawned; /**< The last time a ship has spawned. */
	steady_clock::time_point levelChangePause; /**< The last time a level has ended and triggered a level change pause.*/
public:
	LevelSystem();
	bool isLevelActive; /**< A bool that says whether a level is active. */
	
	unsigned int getAmountOfAsteroidSpawns();
	void nextLevel(steady_clock::time_point currentTickTime);
	bool canShipSpawn(steady_clock::time_point currentTickTime);
	bool canStartLevel(steady_clock::time_point currentTickTime);
	void setBeginTime(steady_clock::time_point currentTickTime);
	int getCurrentLevel();
};