#include "SmallShipModel.h"

float SmallShipModel::radius = 6.0f;

/**
 * Creates a small ship.
 * 
 * \param startingPosition The initial x,y,z position of the ship in world space.
 * \param timeOfSpawn The time point of the current tick the ship is being created in.
 * \param startOnLeft A bool that says whether the ship starts on the left and will move LTR or vice versa.
 */
SmallShipModel::SmallShipModel(float startingPosition[2], steady_clock::time_point timeOfSpawn, bool startOnLeft) //, float inaccuracyInRad
	: BaseShipModel(startingPosition, timeOfSpawn, startOnLeft, ActorType::ShipSmall), shootingInaccuracy(0.0f) {
}

SmallShipModel::~SmallShipModel() {
}

/**
 * Calculates the direction its next projectile should fire towards.
 *
 * Small ships fire at the player with slight inaccuracy, decreasing over
 * each level until they fire at the current position of the player in
 * the current tick.
 * 
 * They can not predict the player's movement, making it dodgeable as long
 * as the player keeps moving.
 *
 * \param playerPosition The position of the player.
 * \return The direction to fire towards as a radian value.
 */
float SmallShipModel::calcProjectileDirection(float playerPosition[2]) {
	float preciseDirection = -atan2(playerPosition[1] - this->position[1], playerPosition[0] - this->position[0]) + MY_PI / 2;

	bool missLeftOrRight = randomBool();
	return preciseDirection += missLeftOrRight ? this->shootingInaccuracy : -(this->shootingInaccuracy); //links oder rechts unakkurat
}

/**
 * Changes the inaccuracy value of the ship's blaster.
 * 
 * It is used for increasing its accuracy as the player
 * progresses through levels.
 * 
 * \param newInaccuracy A radian angle a shot misses by.
 */
void SmallShipModel::setInaccuracy(float newInaccuracy) {
	this->shootingInaccuracy = newInaccuracy;
}

unsigned int SmallShipModel::getPointsValue() {
	return 1000;
}
