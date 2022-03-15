#pragma once

#include <string>

#include "ActorTypes.h"
#include "../IDGen.h"

/**
 * Base class for every in-game actor. 
 *
 * Includes position data.
 */
class ActorModel {
public:
	unsigned int id; /**< The unique id of this game object. */
	float position[2]; /**< The actor's three-dimensional position in world space. */
	float rotation; /**< The orientation of the actor. */
	ActorType actorType; /**< The type of object it represents. */

	ActorModel(float startingPosition[2], float rotation, ActorType actorType);
	~ActorModel();

	virtual unsigned int getPointsValue();
};