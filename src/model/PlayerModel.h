#pragma once
#include "ActorModel.h"

#include <iostream>

#include <chrono>
using namespace std::chrono;

class PlayerModel : public ActorModel {
public:
	static float radius;
	steady_clock::time_point projectileCooldown;
	unsigned int activeProjectileCount;

	PlayerModel(float startingPosition[3], float rotation);
	~PlayerModel();

	void MoveForward();
	void RotateRight();
	void RotateLeft();
	//void initHyperspace();

	void hasBeenHit();
	bool fireProjectileIfOffCooldown(steady_clock::time_point currentTime);
	unsigned int getPointsValue();
};