#pragma once

#include <model/Maths.h>

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
		//positions
		float x;
		float y;

		float direction;
		float rotation;

		float collisionRadius;

		float deltaVelocity;
		float deacceleration;

		float currentSpeed;
		float maxSpeed;
		
		float velocityX;
		float velocityY;
		unsigned int id;
		
		bool boundInWindowX;
		bool boundInWindowY;

		AccelerationType accelerationType;
		PhysicsObject(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius, AccelerationType accelerationType);
		~PhysicsObject();
	};

private:
	std::shared_ptr<PhysicsObject> player;
	std::vector<std::shared_ptr<PhysicsObject>> actorPhysicsObjects;
	std::vector<std::shared_ptr<PhysicsObject>> actorsWithHitboxRegistration;

	unsigned int boundaryX;
	unsigned int boundaryY;
public:
	PhysicsEngine(unsigned int boundaryX, unsigned int boundaryY);
	~PhysicsEngine();
	void addActor(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius, bool hasHitboxRegistration, AccelerationType accelerationType);
	void addPlayer(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius);
	void removeActor(unsigned int id);

	void setDirection(unsigned int id, float directionInRad);
	void setAcceleration(unsigned int id, float acceleration);
	void setBoundByWindow(unsigned int id, bool x, bool y);


	void rotatePlayerLeft();
	void rotatePlayerRight();

	std::vector <std::tuple<unsigned int, float, float, float>> updatePositions(unsigned int ticksPassed);
	std::vector<std::pair<unsigned int, unsigned int>> checkCollisions(); //returns pairs of ids with colliding objects
};