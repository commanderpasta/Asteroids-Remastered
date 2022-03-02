#pragma once
#include "ActorModel.h"

#include <iostream>

#include <chrono>
using namespace std::chrono;

class PlayerModel : public ActorModel {
public:
	static float radius;
	steady_clock::time_point projectileCooldown;
	steady_clock::time_point lastTimeBoosterWasSwitched;
	unsigned int activeProjectileCount;
	bool isAccelerating;
	bool isBoosterActive;

	PlayerModel(float startingPosition[3], float rotation);
	~PlayerModel();

	void hasBeenHit();
	bool fireProjectileIfOffCooldown(steady_clock::time_point currentTime);
	unsigned int getPointsValue();
	void checkBoosterActive(steady_clock::time_point currentTime);
};