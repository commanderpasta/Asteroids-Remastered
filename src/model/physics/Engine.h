#pragma once

#include <Maths.h>

#include <memory>
#include <vector>
#include <iostream>

#define DELTA 0.03f
#define TICK_RATE 60

/**
 * Used to differentiate between different acceleration models for a game object.
 *
 */
enum class AccelerationType { Linear, Constant };

/**
 * Manages the physical properties of game objects.
 *
 * Contains movement calculations and collision detection.
 */
class PhysicsEngine {
	/**
	 * Describes the physical properties of a game object.
	 */
	class PhysicsObject {
	public:
		float x; /**< The object's x-coordinate in the playing space. */
		float y; /**< The object's y-coordinate in the playing space. */

		float direction; /**< The orientation the object is moving towards. */
		float rotation; /**< The orienation the object is facing. */

		float collisionRadius; /**< The radius around the object where it detects a collision. */

		float deltaVelocity; /**< The velocity that is applied to the object every tick. */
		float deacceleration; /**< The amount it deccelerates by every tick. */

		float currentSpeed; /**< The absolute speed the object is moving at. */
		float maxSpeed; /**< The maximum allowed speed for the object. */
		
		float velocityX; /**< The velocity of the object towards the x-axis. */
		float velocityY; /**< The velocity of the object towards the y-axis. */
		unsigned int id; /**< A unique id to refer to the object. */
		
		bool boundInWindowX; /**< Decides whether the object wraps around the vertical game space bounds. */
		bool boundInWindowY;  /**< Decides whether the object wraps around the horizontal game space bounds. */

		AccelerationType accelerationType; /**< Describes whether the acceleration of the object has physical properties. */
		PhysicsObject(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius, AccelerationType accelerationType);
		~PhysicsObject();
	};

private:
	std::vector<std::shared_ptr<PhysicsObject>> actorPhysicsObjects; /**< Contains every <PhysicsObject> to itervate over. */
	std::vector<std::shared_ptr<PhysicsObject>> actorsWithHitboxRegistration; /**< Contains every <PhysicsObject> that announces collision detections every tick. */

	unsigned int boundaryX; /**< The x-axis bounds of the game space. */
	unsigned int boundaryY; /**< The y-axis bounds of the game space. */
public:
	PhysicsEngine(unsigned int boundaryX, unsigned int boundaryY);
	~PhysicsEngine();
	void addActor(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius, bool hasHitboxRegistration, AccelerationType accelerationType);
	void removeActor(unsigned int id);

	void setDirection(unsigned int id, float directionInRad);
	void setAcceleration(unsigned int id, float acceleration);
	void setBoundByWindow(unsigned int id, bool x, bool y);


	void rotateObjectLeft(unsigned int id);
	void rotateObjectRight(unsigned int id);

	std::vector <std::tuple<unsigned int, float, float, float>> updatePositions();
	std::vector<std::pair<unsigned int, unsigned int>> checkCollisions();
};