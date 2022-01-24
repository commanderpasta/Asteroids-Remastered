#pragma once
#include "ActorModel.h"

class AsteroidModel : public ActorModel {
public:
	static float radius;
	unsigned int getPointsValue();
	AsteroidModel(float startingPosition[3]);
	~AsteroidModel();

	void hasBeenHit();
};