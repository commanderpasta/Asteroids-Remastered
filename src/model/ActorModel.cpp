#include "ActorModel.h"

unsigned int idCount = 0;

ActorModel::ActorModel(float startingPosition[3], float rotation, std::string texturePath, std::string shaderPath, std::string type, Shape shape)
	: position{ startingPosition[0], startingPosition[1], startingPosition[2] }, id(idCount), rotation(rotation), texturePath(texturePath), shaderPath(shaderPath), 
	shape(shape), type(type) {
	idCount++;
}

ActorModel::~ActorModel() {
}

void ActorModel::hasBeenHit() {
}