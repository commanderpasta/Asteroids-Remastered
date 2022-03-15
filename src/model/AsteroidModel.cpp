#include "AsteroidModel.h"

float AsteroidModel::radius = 30.0f;

/**
 * Creates an asteroid.
 * 
 * \param startingPosition The initial x,y,z position of the asteroid in world space.
 */
AsteroidModel::AsteroidModel(float startingPosition[2])
	: ActorModel(startingPosition, 0, ActorType::AsteroidLarge) {
}

AsteroidModel::~AsteroidModel() {
}

unsigned int AsteroidModel::getPointsValue() {
	return 20;
}