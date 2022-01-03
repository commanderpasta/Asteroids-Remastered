#pragma once
#include "ActorModel.h"

class AsteroidModel : public ActorModel {
public:
	static float radius;
	AsteroidModel(float startingPosition[3]);
	~AsteroidModel();

	void hasBeenHit();
};