#include "BaseShipModel.h"

float BaseShipModel::radius = 20.0f;
BaseShipModel::BaseShipModel(float startingPosition[3], steady_clock::time_point timeOfSpawn, bool isDirectionLeft)
	: ActorModel(startingPosition, 0, "res/textures/spaceshipNeu.bmp", "res/shaders/Basic.shader", "ship", Shape::Quad), lastChangeOfDirection(timeOfSpawn) {
	if (isDirectionLeft) {
		this->movingState = MovingState::Left;
	} else {
		this->movingState = MovingState::Right;
	}
}

BaseShipModel::~BaseShipModel() {
}

MovingState BaseShipModel::changeDirection() {
	bool upOrDown = rand() % 2;

	switch (this->movingState) {
	case MovingState::Left:
		if (upOrDown) {
			this->movingState = MovingState::LeftUp;
		} else {
			this->movingState = MovingState::LeftDown;
		}
		break;
	case MovingState::LeftUp:
		this->movingState = MovingState::Left;
		break;
	case MovingState::LeftDown:
		this->movingState = MovingState::Left;
		break;

	case MovingState::Right:
		if (upOrDown) {
			this->movingState = MovingState::RightUp;
		}
		else {
			this->movingState = MovingState::RightDown;
		}
		break;
	case MovingState::RightUp:
		this->movingState = MovingState::Right;
		break;
	case MovingState::RightDown:
		this->movingState = MovingState::Right;
		break;
	default:
		break;
	}

	return this->movingState;
}

