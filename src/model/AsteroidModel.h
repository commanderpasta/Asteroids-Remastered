#pragma once
#include "ActorModel.h"

/**
 * Extends upon the <ActorModel> to represent a large asteroid.
 */
class AsteroidModel : public ActorModel {
public:
	static float radius;
	unsigned int getPointsValue();
	AsteroidModel(float startingPosition[3]);
	~AsteroidModel();

	void hasBeenHit();
};