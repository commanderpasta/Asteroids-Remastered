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
	unsigned int id; /**< The unique id of this game object. */
	float position[3]; /**< The actor's three-dimensional position in world space. */
	float rotation; /**< The orientation of the actor. */
	ActorType actorType; /**< The type of object it represents. */

	ActorModel(float startingPosition[3], float rotation, ActorType actorType);
	~ActorModel();

	virtual unsigned int getPointsValue();
};