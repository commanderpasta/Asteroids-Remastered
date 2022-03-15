#include "MediumAsteroidModel.h"


float MediumAsteroidModel::radius = 15.0f;

/**
 * Creates a medium-sized asteroid.
 *
 * \param startingPosition The initial x,y,z position of the asteroid in world space.
 */
MediumAsteroidModel::MediumAsteroidModel(float startingPosition[2])
	: ActorModel(startingPosition, 0, ActorType::AsteroidMedium) {
}

MediumAsteroidModel::~MediumAsteroidModel() {
}

unsigned int MediumAsteroidModel::getPointsValue() {
	return 50;
}