#include "LargeShipModel.h"

float LargeShipModel::radius = 10.0f;

LargeShipModel::LargeShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool startOnLeft) 
	: BaseShipModel(startingPosition, timeOfSpawn, startOnLeft, ActorType::ShipLarge) {
}

LargeShipModel::~LargeShipModel() {
}

float LargeShipModel::calcProjectileDirection(float playerPosition[3]) {
	return getRandomFloat(0.0f, MY_PI * 2);
}

unsigned int LargeShipModel::getPointsValue() {
	return 200;
}


