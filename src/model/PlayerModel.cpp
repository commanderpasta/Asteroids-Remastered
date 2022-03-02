#include "PlayerModel.h"

float PlayerModel::radius = 9.0f;

PlayerModel::PlayerModel(float startingPosition[3], float rotation)
	: ActorModel(startingPosition, rotation, ActorType::Player), projectileCooldown(std::chrono::seconds(1)), activeProjectileCount(0), isAccelerating(false), isBoosterActive(false) {
}

PlayerModel::~PlayerModel() {

}

void PlayerModel::hasBeenHit() {
	std::cout << "RIP" << std::endl;
}

bool PlayerModel::fireProjectileIfOffCooldown(steady_clock::time_point currentFrameTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentFrameTime - this->projectileCooldown);
	if (timeSpan.count() > 0.25) { //this->projectileSize < 4
		this->projectileCooldown = currentFrameTime;

		return true;
	}

	return false;
}

unsigned int PlayerModel::getPointsValue() {
	return 0;
}

void PlayerModel::checkBoosterActive(steady_clock::time_point currentFrameTime) {
	if (this->isAccelerating) {
		duration<double> timeSpan = duration_cast<duration<double>>(currentFrameTime - this->lastTimeBoosterWasSwitched);

		if (timeSpan.count() > 0.1) {
			this->lastTimeBoosterWasSwitched = currentFrameTime;
			this->isBoosterActive = !this->isBoosterActive;
		}
	}
	else {
		this->isBoosterActive = false;
	}
}