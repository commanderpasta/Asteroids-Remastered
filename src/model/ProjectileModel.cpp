#include "ProjectileModel.h"

float ProjectileModel::radius = 5.0f;

ProjectileModel::ProjectileModel(float startingPosition[3], steady_clock::time_point currentTime, unsigned int ownerId)
	: ActorModel(startingPosition, 0, ActorType::Projectile), timeOfSpawn(currentTime), ownerId(ownerId) {
}

ProjectileModel::~ProjectileModel() {
}

void ProjectileModel::hasBeenHit() {
	//award points und so
}

unsigned int ProjectileModel::getPointsValue() {
	return 0;
}