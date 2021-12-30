#include "PlayerModel.h"

PlayerModel::PlayerModel(float startingPosition[3], float rotation)
	: ActorModel(startingPosition, rotation, "res/textures/spaceshipNeu.bmp", "res/shaders/Basic.shader", "player", Shape::Triangle) {
}

PlayerModel::~PlayerModel() {

}

void PlayerModel::MoveForward() {
	float x = 2.0f * sin(this->rotation) + this->position[0];
	float y = 2.0f * cos(this->rotation) + this->position[1];
}
