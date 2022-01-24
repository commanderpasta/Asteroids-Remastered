#pragma once
#include "ActorModel.h"

class MediumAsteroidModel : public ActorModel {
public:
	static float radius;
	MediumAsteroidModel(float startingPosition[3]);
	~MediumAsteroidModel();

	void hasBeenHit();
	unsigned int getPointsValue();
};