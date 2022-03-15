#pragma once

#include "BaseShipModel.h"

#include "Maths.h"

#include <chrono>
using namespace std::chrono;

/**
 * Extends upon the BaseShipModel to represent a large ship.
 *
 * Contains the firing logic to support firing in random directions.
 */
class LargeShipModel : public BaseShipModel {
public:
	static float radius;

	LargeShipModel(float startingPosition[2], steady_clock::time_point timeOfSpawn, bool isDirectionLeft);
	~LargeShipModel();

	float calcProjectileDirection(float playerPosition[2]);
	unsigned int getPointsValue();
};
