#include "ProjectileModel.h"

float ProjectileModel::radius = 2.0f;

/**
 * Spawns a projectile.
 * 
 * \param startingPosition The initial x,y,z position of the projectile in world space.
 * \param currentTickTime The time of the current game tick.
 * \param ownerId The id of the object that fired the projectile.
 */
ProjectileModel::ProjectileModel(float startingPosition[2], steady_clock::time_point currentTickTime, unsigned int ownerId)
	: ActorModel(startingPosition, 0, ActorType::Projectile), timeOfSpawn(currentTickTime), ownerId(ownerId) {
}

ProjectileModel::~ProjectileModel() {
}

unsigned int ProjectileModel::getPointsValue() {
	return 0;
}