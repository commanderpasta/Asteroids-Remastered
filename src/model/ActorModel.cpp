  #include "ActorModel.h"
/**
 * Creates a new game object.
 * 
 * Used as a base class for various actor types to provide shared functionality.
 * 
 * \param startingPosition The object's initial position in world space.
 * \param rotation The object's initial orientation.
 * \param actorType The type of object it represents.
 */
ActorModel::ActorModel(float startingPosition[2], float rotation, ActorType actorType)
	: position{ startingPosition[0], startingPosition[1] }, id(generateId()), rotation(rotation), actorType(actorType) {
}

ActorModel::~ActorModel() {
}
 
/**
 * Returns the points this object awards when the player kills it.
 * 
 * \return Returns the amount of points to award.
 */
unsigned int ActorModel::getPointsValue()
{
	return 0;
}