#include "PlayerModel.h"

const float DELTA = 0.03f;

PlayerModel::PlayerModel(float startingPosition[3], float rotation, int boundaryX, int boundaryY)
	: ActorModel(startingPosition, rotation, "res/textures/spaceshipNeu.bmp", "res/shaders/Basic.shader", "player", Shape::Triangle, boundaryX, boundaryY) {
}

PlayerModel::~PlayerModel() {

}

void PlayerModel::MoveForward() {
	float x = 2.0f * sin(this->rotation) + this->position[0];
	float y = 2.0f * cos(this->rotation) + this->position[1];
	this->SetPosition(x, y);
}

void PlayerModel::RotateLeft() {
	this->rotation -= DELTA;
	if (this->rotation <= 0.0f) {
		this->rotation = 2 * MY_PI;
	}
}

void PlayerModel::RotateRight() {
	this->rotation += DELTA;
	if (rotation >= (2 * MY_PI)) {
		rotation = 0.0f;
	}
}
