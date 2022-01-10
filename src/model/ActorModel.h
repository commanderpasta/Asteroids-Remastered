#pragma once

#include "ActorTypes.h"

#include <string>

class ActorModel {
public:
	unsigned int id;
	float position[3];
	float rotation;
	ActorType actorType;

	ActorModel(float startingPosition[3], float rotation, ActorType actorType);
	~ActorModel();

	virtual void hasBeenHit();
};