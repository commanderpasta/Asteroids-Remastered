#include "ActorModel.h"

unsigned int idCount = 0;

ActorModel::ActorModel(float startingPosition[3], float rotation, std::string texturePath, std::string shaderPath, Shape shape, int boundaryX, int boundaryY)
	: position{ startingPosition[0], startingPosition[1], startingPosition[2] }, id(idCount), rotation(rotation), texturePath(texturePath), shaderPath(shaderPath), 
	shape(shape), boundaryX(boundaryX), boundaryY(boundaryY) {
	idCount++;
}

ActorModel::~ActorModel() {
}

void ActorModel::SetPosition(float x, float y) {
	if (x > this->boundaryX) {
		x -= this->boundaryX;
	} else if (x < 0) {
		x += this->boundaryX;
	}

	if (y > this->boundaryY) {
		y -= this->boundaryY;
	}
	else if (y < 0) {
		y += this->boundaryY;
	}


	this->position[0] = x;
	this->position[1] = y;
}
