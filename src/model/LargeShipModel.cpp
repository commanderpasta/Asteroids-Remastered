#include "LargeShipModel.h"

LargeShipModel::LargeShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool isDirectionLeft) 
	: BaseShipModel(startingPosition, timeOfSpawn, isDirectionLeft, ActorType::ShipLarge) {
}

LargeShipModel::~LargeShipModel() {
}

