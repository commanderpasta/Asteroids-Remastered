#pragma once
#include "ActorModel.h"

#include <chrono>

using namespace std::chrono;

/**
 * Extends upon the ActorModel to represent any projectile.
 * 
 * Can either belong to the player or a ship.
 */
class ProjectileModel : public ActorModel {
public:
	steady_clock::time_point timeOfSpawn; /**< The time of when the projectile has spawned. */
	static float radius;
	unsigned int ownerId; /**< The id of the object that fired the projectile. */
	ProjectileModel(float startingPosition[3], steady_clock::time_point currentTickTime, unsigned int ownerId);
	~ProjectileModel();

	unsigned int getPointsValue();
};