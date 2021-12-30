#pragma once
#include "ActorModel.h"

class AsteroidModel : public ActorModel {
public:
	float direction; //not to confuse with rotation, the texture is not supposed to rotate
	AsteroidModel(float startingPosition[3]);
	~AsteroidModel();
};