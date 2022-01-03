#include "PlayerModel.h"

float PlayerModel::radius = 10.0f;

PlayerModel::PlayerModel(float startingPosition[3], float rotation)
	: ActorModel(startingPosition, rotation, "res/textures/spaceshipNeu.bmp", "res/shaders/Basic.shader", "player", Shape::Triangle) {
}

PlayerModel::~PlayerModel() {

}

void PlayerModel::MoveForward() {
	float x = 2.0f * sin(this->rotation) + this->position[0];
	float y = 2.0f * cos(this->rotation) + this->position[1];
}

void PlayerModel::hasBeenHit() {
	std::cout << "RIP" << std::endl;
}