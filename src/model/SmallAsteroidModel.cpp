#include "SmallAsteroidModel.h"

float SmallAsteroidModel::radius = 20.0f;
SmallAsteroidModel::SmallAsteroidModel(float startingPosition[3])
	: ActorModel(startingPosition, 0, ActorType::AsteroidSmall) {
}

SmallAsteroidModel::~SmallAsteroidModel() {
}

void SmallAsteroidModel::hasBeenHit() {
	//award points und so
}