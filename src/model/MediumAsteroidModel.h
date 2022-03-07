#pragma once
#include "ActorModel.h"

/**
 * Extends upon the ActorModel to represent a medium-sized asteroid.
 */
class MediumAsteroidModel : public ActorModel {
public:
	static float radius;
	MediumAsteroidModel(float startingPosition[3]);
	~MediumAsteroidModel();

	unsigned int getPointsValue();
};