#pragma once

#include "ActorModel.h"

#include <memory>
#include <chrono>
using namespace std::chrono;

/**
 * Extends upon the ActorModel to represent particles.
 * 
 * Particles appear when objects like ships, the player or asteroids get destroyed. They have a limited lifetime.
 */
class ParticleModel : public ActorModel {
	steady_clock::time_point timeOfSpawn; /**< The time of when the particle has spawned. */
public:
	ParticleModel(steady_clock::time_point timeOfSpawn, float sourcePosition[2], ActorType sourceType);
	~ParticleModel();

	bool shouldDestroy(steady_clock::time_point currentTickTime);
};
