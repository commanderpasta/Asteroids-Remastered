#pragma once

#include <memory>
#include <vector>

#define MY_PI 3.14159265358979323846
#define DELTA 0.03f

class PhysicsEngine {
	class PhysicsObject {
	public:
		//positions
		float x;
		float y;

		float direction;
		float rotation;

		float acceleration;

		bool doesDeaccelerate;
		
		float xForce;
		float yForce;
		unsigned int id;
		PhysicsObject(unsigned int id, float x, float y, float direction, float acceleration, bool doesDeaccelerate);
		~PhysicsObject();
	};

private: 
	std::vector<std::shared_ptr<PhysicsObject>> actorPhysicsObjects;
	std::shared_ptr<PhysicsObject> player;

	unsigned int boundaryX;
	unsigned int boundaryY;
public:
	PhysicsEngine(unsigned int boundaryX, unsigned int boundaryY);
	~PhysicsEngine();
	void addActor(unsigned int id, float x, float y, float direction, float acceleration);
	void addPlayer(unsigned int id, float x, float y, float direction, float acceleration);
	void removeActor(unsigned int id);

	//void setPlayerDirection(float direction, bool isAccelerating);
	void setPlayerAccelerating(bool isAccelerating);
	void rotatePlayerLeft();
	void rotatePlayerRight();

	std::vector <std::tuple<unsigned int, float, float, float>> updatePositions();
	std::vector<unsigned int[2]> checkCollisions(); //returns pairs of ids with colliding objects
};