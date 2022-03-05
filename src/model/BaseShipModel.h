#pragma once

#include "ActorModel.h"

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
	MovingState movingState;
	MovingState baseDirection;
public:
	static float radius;
	unsigned int activeProjectileCount;

	steady_clock::time_point projectileCooldown;
	steady_clock::time_point lastChangeOfDirection;
	MovingState changeDirection();

	BaseShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool startOnLeft, ActorType shipType);
	~BaseShipModel();

	bool hasReachedOtherSide(int windowX);
	virtual float calcProjectileDirection(float playerPosition[3]) = 0;
};
