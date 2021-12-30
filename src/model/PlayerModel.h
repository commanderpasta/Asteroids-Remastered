#pragma once
#include "ActorModel.h"

class PlayerModel : public ActorModel {
public:
	PlayerModel(float startingPosition[3], float rotation);
	~PlayerModel();

	void MoveForward();
	void RotateRight();
	void RotateLeft();
	//void initHyperspace();
};