#pragma once
#include "ActorModel.h"

class ProjectileModel : public ActorModel {
public:
	static float radius;
	ProjectileModel(float startingPosition[3]);
	~ProjectileModel();

	void hasBeenHit();
};