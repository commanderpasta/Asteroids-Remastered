#include "LargeShipModel.h"

float LargeShipModel::radius = 10.0f;

/**
 * Spawns a large ship.
 * 
 * \param startingPosition The initial x,y,z position of the ship in world space.
 * \param timeOfSpawn The time point of the current tick the ship is being created in.
 * \param startOnLeft A bool that says whether the ship starts on the left and will move LTR or vice versa.
 */
LargeShipModel::LargeShipModel(float startingPosition[2], steady_clock::time_point timeOfSpawn, bool startOnLeft) 
	: BaseShipModel(startingPosition, timeOfSpawn, startOnLeft, ActorType::ShipLarge) {
}

LargeShipModel::~LargeShipModel() {
}

/**
 * Calculates the direction its next projectile should fire towards.
 * 
 * Large ships fire in an entirely random direction, ignoring the
 * current position of the player.
 * 
 * \param playerPosition The position of the player.
 * \return A random direction as a radian value.
 */
float LargeShipModel::calcProjectileDirection(float playerPosition[2]) {
	return getRandomFloat(0.0f, MY_PI * 2);
}

unsigned int LargeShipModel::getPointsValue() {
	return 200;
}


