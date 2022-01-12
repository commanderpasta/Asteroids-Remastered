#pragma once

#include "ActorModel.h"

#include <chrono>
using namespace std::chrono;

enum class MovingState { Left, Right, LeftUp, LeftDown, RightUp, RightDown };

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

	bool fireProjectileIfOffCooldown(steady_clock::time_point currentTime);
	bool hasReachedOtherSide(int windowX);
};
