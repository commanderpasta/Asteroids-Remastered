#pragma once

#include <chrono>
using namespace std::chrono;

class LevelSystem {
	unsigned int currentLevel;
	steady_clock::time_point lastTimeShipSpawned;
	steady_clock::time_point levelChangePause;
public:
	LevelSystem();
	bool isLevelActive;
	
	unsigned int getAmountOfAsteroidSpawns();
	void nextLevel(steady_clock::time_point currentFrameTime);
	bool canShipSpawn(steady_clock::time_point currentFrameTime);
	bool canStartLevel(steady_clock::time_point currentFrameTime);
	void setBeginTime(steady_clock::time_point currentFrameTime);
	int getCurrentLevel();
};