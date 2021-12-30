#include "AsteroidModel.h"

AsteroidModel::AsteroidModel(float startingPosition[3])
	: ActorModel(startingPosition, 0, "res/textures/Asteroid1.bmp", "res/shaders/Basic.shader", "asteroid", Shape::Quad) {
}

AsteroidModel::~AsteroidModel() {
}