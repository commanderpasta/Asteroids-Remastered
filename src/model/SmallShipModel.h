#pragma once

#include "BaseShipModel.h"

#include <chrono>
using namespace std::chrono;

class SmallShipModel : public BaseShipModel {
public:
	SmallShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool isDirectionLeft);
	~SmallShipModel();
};
