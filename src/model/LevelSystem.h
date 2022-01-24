#pragma once

#include <chrono>
using namespace std::chrono;

class LevelSystem {
	unsigned int currentLevel;
	steady_clock::time_point lastTimeShipSpawned;
	steady_clock::time_point levelChangePause;
public:
	LevelSystem(steady_clock::time_point currentTime);
	bool isLevelActive;
	
	unsigned int getAmountOfAsteroidSpawns();
	void nextLevel(steady_clock::time_point currentTime);
	bool canShipSpawn(steady_clock::time_point currentTime);
	bool canStartLevel(steady_clock::time_point currentTime);
};