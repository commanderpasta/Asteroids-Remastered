#pragma once
#include "ActorModel.h"

/**
 * Extends upon the <ActorModel> to represent a large asteroid.
 */
class AsteroidModel : public ActorModel {
public:
	static float radius; /**< The object's size. */
	unsigned int getPointsValue();
	
	AsteroidModel(float startingPosition[3]);
	~AsteroidModel();
};