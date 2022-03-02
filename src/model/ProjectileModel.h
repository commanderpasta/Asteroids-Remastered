#pragma once
#include "ActorModel.h"

#include <chrono>

using namespace std::chrono;

class ProjectileModel : public ActorModel {
public:
	steady_clock::time_point timeOfSpawn;
	static float radius;
	unsigned int ownerId;
	ProjectileModel(float startingPosition[3], steady_clock::time_point currentFrameTime, unsigned int ownerId);
	~ProjectileModel();

	void hasBeenHit();
	unsigned int getPointsValue();
};