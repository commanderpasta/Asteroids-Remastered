#pragma once

#include <memory>
#include <vector>
#include <iostream>

#include <model/Maths.h>

#define DELTA 0.03f

enum class AccelerationType { Linear, None };

class PhysicsEngine {
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

	std::vector <std::tuple<unsigned int, float, float, float>> updatePositions();
	std::vector<std::pair<unsigned int, unsigned int>> checkCollisions(); //returns pairs of ids with colliding objects
};