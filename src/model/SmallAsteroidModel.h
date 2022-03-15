#pragma once
#include "ActorModel.h"

/**
 * Extends upon the ActorModel to represent a small asteroid.
 */
class SmallAsteroidModel : public ActorModel {
public:
	static float radius;
	SmallAsteroidModel(float startingPosition[2]);
	~SmallAsteroidModel();

	unsigned int getPointsValue();
};