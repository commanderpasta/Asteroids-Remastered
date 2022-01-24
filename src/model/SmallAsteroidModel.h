#pragma once
#include "ActorModel.h"

class SmallAsteroidModel : public ActorModel {
public:
	static float radius;
	SmallAsteroidModel(float startingPosition[3]);
	~SmallAsteroidModel();

	void hasBeenHit();
	unsigned int getPointsValue();
};