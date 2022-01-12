#include "BaseShipModel.h"

float BaseShipModel::radius = 20.0f;
BaseShipModel::BaseShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool startOnLeft, ActorType shipType)
	: ActorModel(startingPosition, 0, shipType), projectileCooldown(std::chrono::steady_clock::now() - std::chrono::seconds(1)), activeProjectileCount(0) {
	if (startOnLeft) {
		this->movingState = MovingState::Right;
		this->baseDirection = MovingState::Right;
	} else {
		this->movingState = MovingState::Left;
		this->baseDirection = MovingState::Left;
	}
}

BaseShipModel::~BaseShipModel() {
}

MovingState BaseShipModel::changeDirection() {
	bool upOrDown = rand() % 2;

	switch (this->movingState) {
	case MovingState::Left:
		if (upOrDown) {
			this->movingState = MovingState::LeftUp;
		} else {
			this->movingState = MovingState::LeftDown;
		}
		break;
	case MovingState::LeftUp:
		this->movingState = MovingState::Left;
		break;
	case MovingState::LeftDown:
		this->movingState = MovingState::Left;
		break;

	case MovingState::Right:
		if (upOrDown) {
			this->movingState = MovingState::RightUp;
		}
		else {
			this->movingState = MovingState::RightDown;
		}
		break;
	case MovingState::RightUp:
		this->movingState = MovingState::Right;
		break;
	case MovingState::RightDown:
		this->movingState = MovingState::Right;
		break;
	default:
		break;
	}

	return this->movingState;
}

bool BaseShipModel::hasReachedOtherSide(int windowX) {
	return (this->baseDirection == MovingState::Left && this->position[0] <= 0.0f) || (this->baseDirection == MovingState::Right && this->position[0] >= windowX);
}

bool BaseShipModel::fireProjectileIfOffCooldown(steady_clock::time_point currentTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentTime - this->projectileCooldown);
	if (timeSpan.count() > 0.25) { //this->projectileSize < 4
		this->projectileCooldown = currentTime;

		return true;
	}

	return false;
}
