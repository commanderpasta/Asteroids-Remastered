#include "SmallAsteroidModel.h"

float SmallAsteroidModel::radius = 6.0f;

/**
 * Creates a small asteroid.
 *
 * \param startingPosition The initial x,y,z position of the asteroid in world space.
 */
SmallAsteroidModel::SmallAsteroidModel(float startingPosition[2])
	: ActorModel(startingPosition, 0, ActorType::AsteroidSmall) {
}

SmallAsteroidModel::~SmallAsteroidModel() {
}

unsigned int SmallAsteroidModel::getPointsValue() {
	return 100;
}