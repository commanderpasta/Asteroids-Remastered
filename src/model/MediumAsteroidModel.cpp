#include "MediumAsteroidModel.h"

float MediumAsteroidModel::radius = 20.0f;
MediumAsteroidModel::MediumAsteroidModel(float startingPosition[3])
	: ActorModel(startingPosition, 0, "res/textures/Asteroid1.bmp", "res/shaders/Basic.shader", "mediumAsteroid", Shape::Quad) {
}

MediumAsteroidModel::~MediumAsteroidModel() {
}

void MediumAsteroidModel::hasBeenHit() {
	//award points und so
}