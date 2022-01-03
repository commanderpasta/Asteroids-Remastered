#pragma once
#include "ActorModel.h"

#include <iostream>

class PlayerModel : public ActorModel {
public:
	static float radius;
	PlayerModel(float startingPosition[3], float rotation);
	~PlayerModel();

	void MoveForward();
	void RotateRight();
	void RotateLeft();
	//void initHyperspace();

	void hasBeenHit();
};