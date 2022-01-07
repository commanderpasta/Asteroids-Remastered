#include "Engine.h"

#include <iostream>
#include <algorithm>

PhysicsEngine::PhysicsEngine(unsigned int boundaryX, unsigned int boundaryY)
	: boundaryX(boundaryX), boundaryY(boundaryY) {}
PhysicsEngine::~PhysicsEngine() {}

PhysicsEngine::PhysicsObject::PhysicsObject(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius, AccelerationType accelerationType)
	: id(id), x(x), y(y), direction(direction), deltaVelocity(acceleration), deacceleration(deacceleration), maxSpeed(maxSpeed), currentSpeed(startingSpeed), velocityX(startingSpeed * sin(direction)), velocityY(startingSpeed * cos(direction)), rotation(0.0f), collisionRadius(hitboxRadius), accelerationType(accelerationType)
{}
PhysicsEngine::PhysicsObject::~PhysicsObject() {}

void PhysicsEngine::addActor(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius, bool hasHitboxRegistration, AccelerationType accelerationType) {
	std::shared_ptr<PhysicsObject> newActor = std::make_shared<PhysicsObject>(id, x, y, direction, acceleration, deacceleration, startingSpeed, maxSpeed, hitboxRadius, accelerationType);
	this->actorPhysicsObjects.emplace_back(newActor);
	if (hasHitboxRegistration) {
		this->actorsWithHitboxRegistration.emplace_back(newActor);
	}
}

void PhysicsEngine::addPlayer(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius) {
	this->player = std::make_shared<PhysicsObject>(id, x, y, direction, acceleration, deacceleration, startingSpeed, maxSpeed, hitboxRadius, AccelerationType::Linear);
	this->actorPhysicsObjects.emplace_back(this->player);
	this->actorsWithHitboxRegistration.emplace_back(this->player);
}

void PhysicsEngine::removeActor(unsigned int id) {
	auto it = std::find_if(this->actorPhysicsObjects.begin(), this->actorPhysicsObjects.end(), [id](std::shared_ptr<PhysicsObject> object) { return object->id == id; });

	if (it != this->actorPhysicsObjects.end()) { //object found
		this->actorPhysicsObjects.erase(it);


		auto it2 = std::find_if(this->actorsWithHitboxRegistration.begin(), this->actorsWithHitboxRegistration.end(), [id](std::shared_ptr<PhysicsObject> object) { return object->id == id; });

		if (it2 != this->actorsWithHitboxRegistration.end()) {
			this->actorsWithHitboxRegistration.erase(it2);
		}
	} else {
		std::cout << "PhysicsEngine: Actor with id " << id << " can not be found." << std::endl;
	}
}

std::vector <std::tuple<unsigned int, float, float, float>> PhysicsEngine::updatePositions() {

	std::vector <std::tuple<unsigned int, float, float, float>> calculatedPositions;

	for (auto &actor : this->actorPhysicsObjects) {
		actor->currentSpeed = sqrt(pow(actor->velocityX, 2.0f) + pow(actor->velocityY, 2.0f));

		if (actor->accelerationType == AccelerationType::Linear) {
			//actor->velocityX = std::max(0.0f, actor->velocityX - (actor->deacceleration * actor->velocityX / actor->currentSpeed));
			//actor->velocityY = std::max(0.0f, actor->velocityY - actor->deacceleration);

			if (actor->currentSpeed > 0.0f && actor->deacceleration != 0.0f) {
				float velocityXNormalized = actor->velocityX / actor->currentSpeed;
				float velocityYNormalized = actor->velocityY / actor->currentSpeed;

				actor->deacceleration = actor->currentSpeed * 0.005f;
				actor->currentSpeed = std::max(0.0f, actor->currentSpeed - actor->deacceleration);

				actor->velocityX = actor->currentSpeed * velocityXNormalized;
				actor->velocityY = actor->currentSpeed * velocityYNormalized;
			}

			actor->velocityX += actor->deltaVelocity * sin(actor->direction);
			actor->velocityY += actor->deltaVelocity * cos(actor->direction);

			if (actor->currentSpeed > 0.0f) {
				float velocityXNormalized = actor->velocityX / actor->currentSpeed;
				float velocityYNormalized = actor->velocityY / actor->currentSpeed;

				actor->currentSpeed = std::min(actor->maxSpeed, sqrt(pow(actor->velocityX, 2.0f) + pow(actor->velocityY, 2.0f)));

				actor->velocityX = actor->currentSpeed * velocityXNormalized;
				actor->velocityY = actor->currentSpeed * velocityYNormalized;
			}

			//add calculated vector to position
			actor->x += actor->velocityX;
			actor->y += actor->velocityY;
		}
		else {
			actor->x += actor->currentSpeed * cos(actor->direction);
			actor->y += actor->currentSpeed * sin(actor->direction);
		}

		//wrap around if reaching out-of-bounds
		if (actor->x > this->boundaryX) {
			actor->x = fmod(actor->x, this->boundaryX);
		} else if (actor->x < 0.0f) {
			actor->x = this->boundaryX - fmod(actor->x, this->boundaryX);
		}

		if (actor->y > this->boundaryY) {
			actor->y = fmod(actor->y, this->boundaryY);
		} else if (actor->y < 0.0f) {
			actor->y = this->boundaryY - fmod(actor->y, this->boundaryY);
		}
		
		std::tuple<unsigned int, float, float, float> actorPositions {actor->id, actor->x, actor->y, actor->rotation};

		calculatedPositions.push_back(actorPositions);
	}

	return calculatedPositions;
}

void PhysicsEngine::setDirection(unsigned int id, float directionInRad) {
	auto it = std::find_if(this->actorPhysicsObjects.begin(), this->actorPhysicsObjects.end(), [id](std::shared_ptr<PhysicsObject> object) { return object->id == id; });

	if (it != this->actorPhysicsObjects.end()) {
		it->get()->direction = directionInRad;
	}
}

void PhysicsEngine::setAcceleration(unsigned int id, float acceleration) {
	auto it = std::find_if(this->actorPhysicsObjects.begin(), this->actorPhysicsObjects.end(), [id](std::shared_ptr<PhysicsObject> object) { return object->id == id; });

	if (it != this->actorPhysicsObjects.end()) {
		it->get()->deltaVelocity = acceleration;
	}
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
			for (auto& actorWithHitboxRegistration : this->actorsWithHitboxRegistration) {
				if (actor->id != actorWithHitboxRegistration->id) {
					auto dx = (actorWithHitboxRegistration->x + actorWithHitboxRegistration->collisionRadius) - (actor->x + actor->collisionRadius);
					auto dy = (actorWithHitboxRegistration->y + actorWithHitboxRegistration->collisionRadius) - (actor->y + actor->collisionRadius);
					auto distance = sqrt(dx * dx + dy * dy);

					if (distance < (actorWithHitboxRegistration->collisionRadius + actor->collisionRadius)) {
						std::pair<unsigned int, unsigned int> collisionIds = { actorWithHitboxRegistration->id, actor->id };
						test.push_back(collisionIds);
					}
				}
			}
		}
	}
	
	return test;
}
