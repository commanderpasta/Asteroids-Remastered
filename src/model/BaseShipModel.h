#pragma once

#include "ActorModel.h"

#include <chrono>
using namespace std::chrono;

enum class MovingState { Left, Right, LeftUp, LeftDown, RightUp, RightDown };

class BaseShipModel : public ActorModel {
	MovingState movingState;
public:
	static float radius;
	BaseShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool isDirectionLeft);
	~BaseShipModel();

	steady_clock::time_point lastChangeOfDirection;
	MovingState changeDirection();
};
