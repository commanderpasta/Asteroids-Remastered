#pragma once

#include "BaseShipModel.h"

#include "Maths.h"

#include <chrono>
using namespace std::chrono;

class LargeShipModel : public BaseShipModel {
public:
	LargeShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool isDirectionLeft);
	~LargeShipModel();

	float calcProjectileDirection(float playerPosition[3]);
	unsigned int getPointsValue();
};
