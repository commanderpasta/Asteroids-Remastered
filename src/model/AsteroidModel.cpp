#include "AsteroidModel.h"

float AsteroidModel::radius = 20.0f;
AsteroidModel::AsteroidModel(float startingPosition[3])
	: ActorModel(startingPosition, 0, "res/textures/Asteroid1.bmp", "res/shaders/Basic.shader", "asteroid", Shape::Quad) {
}

AsteroidModel::~AsteroidModel() {
}

void AsteroidModel::hasBeenHit() {
	//award points und so
}