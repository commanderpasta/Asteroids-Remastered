#include "PlayerModel.h"

float PlayerModel::radius = 10.0f;

PlayerModel::PlayerModel(float startingPosition[3], float rotation)
	: ActorModel(startingPosition, rotation, ActorType::Player), projectileCooldown(std::chrono::steady_clock::now() - std::chrono::seconds(1)), activeProjectileCount(0) {
}

PlayerModel::~PlayerModel() {

}

void PlayerModel::MoveForward() {
	float x = 2.0f * sin(this->rotation) + this->position[0];
	float y = 2.0f * cos(this->rotation) + this->position[1];
}

void PlayerModel::hasBeenHit() {
	std::cout << "RIP" << std::endl;
}

bool PlayerModel::fireProjectileIfOffCooldown(steady_clock::time_point currentTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentTime - this->projectileCooldown);
	if (timeSpan.count() > 0.25) { //this->projectileSize < 4
		this->projectileCooldown = currentTime;

		return true;
	}

	return false;
}
