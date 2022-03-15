#include "BaseShipModel.h"

float BaseShipModel::radius = 20.0f;

/**
 * Creates a new <BaseShipModel>.
 * 
 * Acts as a base class for the shared functionality between large and small ships.
 * 
 * \param startingPosition The initial x,y,z-position of the ship.
 * \param timeOfSpawn The time point of the current tick the ship is being created in.
 * \param startOnLeft A bool that says whether the ship starts on the left and will move LTR or vice versa.
 * \param shipType Whether it is a large or small ship.
 */
BaseShipModel::BaseShipModel(float startingPosition[2], steady_clock::time_point timeOfSpawn, bool startOnLeft, ActorType shipType)
	: ActorModel(startingPosition, 0, shipType), lastChangeOfDirection(timeOfSpawn), projectileCooldown(timeOfSpawn), activeProjectileCount(0) {
	if (startOnLeft) {
		this->movingState = MovingState::Right;
		this->baseDirection = MovingState::Right;
	} else {
		this->movingState = MovingState::Left;
		this->baseDirection = MovingState::Left;
	}
}

BaseShipModel::~BaseShipModel() {
}

/**
 * Changes the direction of the ship randomly.
 * 
 * It follows a zig-zag pattern where it will alternate between
 * moving laterally and diagonally up or down.
 * 
 * \return Returns the resulting MovingState of the ship.
 */
MovingState BaseShipModel::changeDirection() {
	bool upOrDown = randomBool();

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

/**
 * Checks whether it has reached the other end of the game space.
 * 
 * \param windowX The length of the game bounds.
 * \return A bool that says if it has reached the other side.
 */
bool BaseShipModel::hasReachedOtherSide(int windowX) {
	return (this->baseDirection == MovingState::Left && this->position[0] <= 0.0f) || (this->baseDirection == MovingState::Right && this->position[0] >= windowX);
}
