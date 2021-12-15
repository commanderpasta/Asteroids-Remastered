#include "AsteroidModel.h"

AsteroidModel::AsteroidModel(float startingPosition[3], int boundaryX, int boundaryY)
	: ActorModel(startingPosition, 0, "res/textures/Asteroid1.bmp", "res/shaders/Basic.shader", "asteroid", Shape::Quad, boundaryX, boundaryY) {
	int radianRandUpperLimit = 2 * MY_PI * 100;
	this->direction = rand() % radianRandUpperLimit / 100.0f;
}

AsteroidModel::~AsteroidModel() {
}

void AsteroidModel::Move() {
	float x = 0.5f * sin(this->direction) + this->position[0];
	float y = 0.5f * cos(this->direction) + this->position[1];
	this->SetPosition(x, y);
}