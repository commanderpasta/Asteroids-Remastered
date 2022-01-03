#include "ProjectileModel.h"

float ProjectileModel::radius = 5.0f;
ProjectileModel::ProjectileModel(float startingPosition[3], steady_clock::time_point currentTime)
	: ActorModel(startingPosition, 0, "res/textures/projektil.bmp", "res/shaders/Basic.shader", "projectile", Shape::Quad2), timeOfSpawn(currentTime) {
}

ProjectileModel::~ProjectileModel() {
}

void ProjectileModel::hasBeenHit() {
	//award points und so
}