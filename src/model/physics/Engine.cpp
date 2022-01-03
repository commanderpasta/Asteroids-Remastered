#include "Engine.h"

#include <iostream>
#include <algorithm>

PhysicsEngine::PhysicsEngine(unsigned int boundaryX, unsigned int boundaryY)
	: boundaryX(boundaryX), boundaryY(boundaryY) {}
PhysicsEngine::~PhysicsEngine() {}

PhysicsEngine::PhysicsObject::PhysicsObject(unsigned int id, float x, float y, float direction, float acceleration, float radius, bool doesDeaccelerate)
	: id(id), x(x), y(y), doesDeaccelerate(doesDeaccelerate), direction(direction), acceleration(acceleration), xForce(0.0f), yForce(0.0f), rotation(0.0f), collisionRadius(radius) {}
PhysicsEngine::PhysicsObject::~PhysicsObject() {}

//TODO have maxSpeed as parameter
void PhysicsEngine::addActor(unsigned int id, float x, float y, float direction, float acceleration, float radius) {
	std::shared_ptr<PhysicsObject> newActor = std::make_shared<PhysicsObject>(id, x, y, direction, acceleration, radius, false);
	this->actorPhysicsObjects.emplace_back(newActor);
}

void PhysicsEngine::addPlayer(unsigned int id, float x, float y, float direction, float acceleration, float radius) {
	this->player = std::make_shared<PhysicsObject>(id, x, y, direction, acceleration, radius, true);
	this->actorPhysicsObjects.emplace_back(this->player);
}

void PhysicsEngine::removeActor(unsigned int id) {
	auto it = std::find_if(this->actorPhysicsObjects.begin(), this->actorPhysicsObjects.end(), [id](std::shared_ptr<PhysicsObject> object) { return object->id == id; });

	if (it != this->actorPhysicsObjects.end()) { //object found
		this->actorPhysicsObjects.erase(it);
	} else {
		std::cout << "PhysicsEngine: Actor with id " << id << " can not be found." << std::endl;
	}
}

void PhysicsEngine::setPlayerAccelerating(bool isAccelerating) {
	if (isAccelerating) {
		this->player->acceleration = 0.05f;
	} else {
		this->player->acceleration = 0.0f;
	}
}

std::vector <std::tuple<unsigned int, float, float, float>> PhysicsEngine::updatePositions() {

	std::vector <std::tuple<unsigned int, float, float, float>> bla;

	for (auto &actor : this->actorPhysicsObjects) {
		if (actor->doesDeaccelerate) {
			//apply "breaking" vector
			actor->xForce -= actor->xForce * 0.005f;
			actor->yForce -= actor->yForce * 0.005f;
		}

		//turn in direction
		actor->xForce = std::min(3.0f, actor->xForce + actor->acceleration * sin(actor->direction));
		actor->yForce = std::min(3.0f, actor->yForce + actor->acceleration * cos(actor->direction));

		//add calculated vector to position
		actor->x += actor->xForce;
		actor->y += actor->yForce;

		//wrap around if reaching out-of-bounds
		if (actor->x > this->boundaryX) {
			actor->x -= this->boundaryX;
		}
		else if (actor->x < 0) {
			actor->x += this->boundaryX;
		}

		if (actor->y > this->boundaryY) {
			actor->y -= this->boundaryY;
		}
		else if (actor->y < 0) {
			actor->y += this->boundaryY;
		}

		std::tuple<unsigned int, float, float, float> test {actor->id, actor->x, actor->y, actor->rotation};

		bla.push_back(test);
	}

	return bla;
}

void PhysicsEngine::rotatePlayerLeft() {
	this->player->direction -= DELTA;
	if (this->player->direction <= 0.0f) {
		this->player->direction = 2.0f * MY_PI;
	}

	this->player->rotation = this->player->direction;
}

void PhysicsEngine::rotatePlayerRight() {
	this->player->direction += DELTA;
	if (this->player->direction >= (2 * MY_PI)) {
		this->player->direction = 0.0f;
	}

	this->player->rotation = this->player->direction;
}

std::vector<std::pair<unsigned int, unsigned int>> PhysicsEngine::checkCollisions() {
	std::vector<std::pair<unsigned int, unsigned int>> test;

	for (auto& actor : this->actorPhysicsObjects) {
		if (actor->id != this->player->id) {
			auto dx = (this->player->x + this->player->collisionRadius) - (actor->x + actor->collisionRadius);
			auto dy = (this->player->y + this->player->collisionRadius) - (actor->y + actor->collisionRadius);
			auto distance = sqrt(dx * dx + dy * dy);

			if (distance < (this->player->collisionRadius + actor->collisionRadius)) {
				std::pair<unsigned int, unsigned int> collisionIds = { this->player->id, actor->id };
				test.push_back(collisionIds);
			}
		}
	}
	
	return test;
}
