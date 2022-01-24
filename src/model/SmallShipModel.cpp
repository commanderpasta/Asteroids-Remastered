#include "SmallShipModel.h"

SmallShipModel::SmallShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool startOnLeft)
	: BaseShipModel(startingPosition, timeOfSpawn, startOnLeft, ActorType::ShipSmall), shootingInaccuracy(0.0f) {
}

SmallShipModel::~SmallShipModel() {
}

float SmallShipModel::calcProjectileDirection(float playerPosition[3]) {
	float preciseDirection = -atan2(playerPosition[1] - this->position[1], playerPosition[0] - this->position[0]) + MY_PI / 2;

	bool inaccuracyDirection = randomBool();
	return preciseDirection += inaccuracyDirection ? this->shootingInaccuracy : -(this->shootingInaccuracy);
}

void SmallShipModel::setInaccuracy(float newInaccuracy) {
	this->shootingInaccuracy = newInaccuracy;
}

unsigned int SmallShipModel::getPointsValue() {
	return 1000;
}
