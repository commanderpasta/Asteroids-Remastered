#pragma once

#include "Shapes.h"

#include <string>

class ActorModel {
public:
	unsigned int id;
	float position[3];
	float rotation;
	std::string texturePath;
	std::string shaderPath;
	Shape shape;
	std::string type;

	ActorModel(float startingPosition[3], float rotation, std::string texturePath, std::string shaderPath, std::string type, Shape shape);
	~ActorModel();

	virtual void hasBeenHit();
};