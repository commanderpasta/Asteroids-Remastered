#include "ProjectileModel.h"

float ProjectileModel::radius = 5.0f;
ProjectileModel::ProjectileModel(float startingPosition[3])
	: ActorModel(startingPosition, 0, "res/textures/Asteroid1.bmp", "res/shaders/Basic.shader", "asteroid", Shape::Quad) {
}

ProjectileModel::~ProjectileModel() {
}

void ProjectileModel::hasBeenHit() {
	//award points und so
}