#include "LargeShipModel.h"

LargeShipModel::LargeShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool startOnLeft) 
	: BaseShipModel(startingPosition, timeOfSpawn, startOnLeft, ActorType::ShipLarge) {
}

LargeShipModel::~LargeShipModel() {
}

float LargeShipModel::calcProjectileDirection(float playerPosition[3]) {
	return getRandomFloat(0.0f, MY_PI * 2);
}


