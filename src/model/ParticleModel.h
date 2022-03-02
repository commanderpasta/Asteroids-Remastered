#pragma once

#include "ActorModel.h"

#include <memory>
#include <chrono>
using namespace std::chrono;


class ParticleModel : public ActorModel {
	steady_clock::time_point timeOfSpawn;
public:
	ParticleModel(steady_clock::time_point timeOfSpawn, float sourcePosition[3], ActorType sourceType);
	~ParticleModel();

	bool shouldDestroy(steady_clock::time_point currentFrameTime);
};
