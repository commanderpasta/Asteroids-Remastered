#pragma once

#include "BaseShipModel.h"

#include "Maths.h"

#include <chrono>
using namespace std::chrono;

/**
 * Extends upon the BaseShipModel to represent a small ship.
 *
 * Contains its own firing logic that works separately from the large ship (i.e. firing at the player).
 */
class SmallShipModel : public BaseShipModel {
private:
	float shootingInaccuracy;
public:
	static float radius;

	SmallShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool isDirectionLeft);
	~SmallShipModel();

	float calcProjectileDirection(float playerPosition[3]);
	void setInaccuracy(float newInaccuracy);
	unsigned int getPointsValue();
};
