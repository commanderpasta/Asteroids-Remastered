#include "PlayerModel.h"

float PlayerModel::radius = 9.0f;

/**
 * Creates a ship for the player.
 * 
 * \param startingPosition The initial x,y,z position of the player in world space.
 * \param rotation The initial orientation of the player.
 */
PlayerModel::PlayerModel(float startingPosition[3], float rotation)
	: ActorModel(startingPosition, rotation, ActorType::Player), projectileCooldown(std::chrono::seconds(1)), activeProjectileCount(0), isAccelerating(false), isBoosterActive(false) {
}

PlayerModel::~PlayerModel() {

}

/**
 * Decides whether a projectile should be fired depending on its cooldown.
 * 
 * The player's blaster has a cooldown of 0.25 sceonds.
 * 
 * \param currentTickTime The time of the current game tick.
 * \return A bool that says whether a projectile should be fired.
 */
bool PlayerModel::fireProjectileIfOffCooldown(steady_clock::time_point currentTickTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentTickTime - this->projectileCooldown);
	if (timeSpan.count() > 0.25) {
		this->projectileCooldown = currentTickTime;

		return true;
	}

	return false;
}

unsigned int PlayerModel::getPointsValue() {
	return 0;
}

/**
 * Checks whether the player's booster should thrust.
 * 
 * It alternates every 0.1 seconds while accelerating.
 * 
 * \param currentTickTime The time of the current game tick.
 */
void PlayerModel::checkBoosterActive(steady_clock::time_point currentTickTime) {
	if (this->isAccelerating) {
		duration<double> timeSpan = duration_cast<duration<double>>(currentTickTime - this->lastTimeBoosterWasSwitched);

		if (timeSpan.count() > 0.1) {
			this->lastTimeBoosterWasSwitched = currentTickTime;
			this->isBoosterActive = !this->isBoosterActive;
		}
	}
	else {
		this->isBoosterActive = false;
	}
}