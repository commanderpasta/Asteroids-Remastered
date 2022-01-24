#include "AsteroidModel.h"

float AsteroidModel::radius = 20.0f;

AsteroidModel::AsteroidModel(float startingPosition[3])
	: ActorModel(startingPosition, 0, ActorType::AsteroidLarge) {
}

AsteroidModel::~AsteroidModel() {
}

void AsteroidModel::hasBeenHit() {
	//award points und so
}

unsigned int AsteroidModel::getPointsValue() {
	return 20;
}