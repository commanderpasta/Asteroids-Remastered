#pragma once

#include "BaseShipModel.h"

#include "Maths.h"

#include <chrono>
using namespace std::chrono;

class SmallShipModel : public BaseShipModel {
private:
	float shootingInaccuracy;
public:
	SmallShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool isDirectionLeft);
	~SmallShipModel();

	float calcProjectileDirection(float playerPosition[3]);
	void setInaccuracy(float newInaccuracy);
	unsigned int getPointsValue();
};
