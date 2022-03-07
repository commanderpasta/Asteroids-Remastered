#pragma once
#include "ActorModel.h"

#include <iostream>

#include <chrono>
using namespace std::chrono;

/**
 * Extends upon the ActorModel to represent the ship that is controlled by the player.
 * 
 * It contains logic for how the booster behaved, and manages the cooldown of its projectiles. 
 */
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