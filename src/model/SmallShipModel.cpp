#include "SmallShipModel.h"

SmallShipModel::SmallShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool isDirectionLeft)
	: BaseShipModel(startingPosition, timeOfSpawn, isDirectionLeft) {
}

SmallShipModel::~SmallShipModel() {
}

