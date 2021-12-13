#pragma once

#include "Shapes.h"

#include <string>

#define MY_PI 3.14159265358979323846

class ActorModel {
	int boundaryX;
	int	boundaryY;

protected:
	void SetPosition(float x, float y);
public:
	unsigned int id;
	float position[3];
	float rotation;
	std::string texturePath;
	std::string shaderPath;
	Shape shape;

	ActorModel(float startingPosition[3], float rotation, std::string texturePath, std::string shaderPath, Shape shape, int boundaryX, int boundaryY);
	~ActorModel();
};