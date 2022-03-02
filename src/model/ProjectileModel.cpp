#include "ProjectileModel.h"

float ProjectileModel::radius = 2.0f;

ProjectileModel::ProjectileModel(float startingPosition[3], steady_clock::time_point currentFrameTime, unsigned int ownerId)
	: ActorModel(startingPosition, 0, ActorType::Projectile), timeOfSpawn(currentFrameTime), ownerId(ownerId) {
}

ProjectileModel::~ProjectileModel() {
}

void ProjectileModel::hasBeenHit() {
	//award points und so
}

unsigned int ProjectileModel::getPointsValue() {
	return 0;
}