#pragma once

#include "ActorModel.h"
#include "../Maths.h"

#include <chrono>
using namespace std::chrono;

/**
 * Describes the current state of a ship in the movement loop.
 */
enum class MovingState { Left, Right, LeftUp, LeftDown, RightUp, RightDown };

/**
 * Base class for representing ships in Asteroids.
 * 
 * Contains the movement logic that is shared between large and small ships.
 */
class BaseShipModel : public ActorModel {
	MovingState movingState; /**< Describes the current <MovingState> of the ship. */
	MovingState baseDirection; /**< Describes whether the ship move right to left or left to right. */
public:
	static float radius;
	unsigned int activeProjectileCount; /**< The amount of projectile it launched that have not despawned yet. */

	steady_clock::time_point projectileCooldown; /**< The last time it has fired a projectile. */
	steady_clock::time_point lastChangeOfDirection; /**< Describes the last time its direction has changed. */
	MovingState changeDirection();

	BaseShipModel(float startingPosition[2], steady_clock::time_point timeOfSpawn, bool startOnLeft, ActorType shipType);
	~BaseShipModel();

	bool hasReachedOtherSide(int windowX);
	virtual float calcProjectileDirection(float playerPosition[2]) = 0;
};
