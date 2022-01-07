#include "SmallAsteroidModel.h"

float SmallAsteroidModel::radius = 20.0f;
SmallAsteroidModel::SmallAsteroidModel(float startingPosition[3])
	: ActorModel(startingPosition, 0, "res/textures/Asteroid1.bmp", "res/shaders/Basic.shader", "smallAsteroid", Shape::Quad) {
}

SmallAsteroidModel::~SmallAsteroidModel() {
}

void SmallAsteroidModel::hasBeenHit() {
	//award points und so
}