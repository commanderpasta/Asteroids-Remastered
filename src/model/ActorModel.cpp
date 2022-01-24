#include "ActorModel.h"

unsigned int idCount = 0;

ActorModel::ActorModel(float startingPosition[3], float rotation, ActorType actorType)
	: position{ startingPosition[0], startingPosition[1], startingPosition[2] }, id(idCount), rotation(rotation), actorType(actorType) {
	idCount++;
}

ActorModel::~ActorModel() {
}

unsigned int ActorModel::getPointsValue()
{
	return 0;
}

void ActorModel::hasBeenHit() {
}