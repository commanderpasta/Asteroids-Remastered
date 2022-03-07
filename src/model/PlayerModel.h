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
	steady_clock::time_point projectileCooldown; /**< The last time it has fired a projectile. */
	steady_clock::time_point lastTimeBoosterWasSwitched; /**< The last time the player's booster has bursted. */
	unsigned int activeProjectileCount;  /**< The amount of projectile it launched that have not despawned yet. */
	bool isAccelerating; /**< A bool that described whether the player is accelerating. */
	bool isBoosterActive; /**< Describes whether the player's booster is bursting. */

	PlayerModel(float startingPosition[3], float rotation);
	~PlayerModel();

	bool fireProjectileIfOffCooldown(steady_clock::time_point currentTime);
	unsigned int getPointsValue();
	void checkBoosterActive(steady_clock::time_point currentTime);
};