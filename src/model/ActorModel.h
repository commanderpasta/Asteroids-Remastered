#pragma once

#include "ActorTypes.h"

#include <string>

/**
 * Base class for every in-game actor. 
 *
 * Includes position data.
 */
class ActorModel {
public:
	unsigned int id;
	float position[3];
	float rotation;
	ActorType actorType;

	ActorModel(float startingPosition[3], float rotation, ActorType actorType);
	~ActorModel();

	virtual unsigned int getPointsValue();
	virtual void hasBeenHit();
};