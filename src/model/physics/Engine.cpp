#include "Engine.h"

#include <iostream>
#include <algorithm>

/**
 * Creates an instance of a physics engine that manages <PhysicsObject>'s.
 * 
 * It functions as a manager that doesn't have direct knowledge of the object's
 * properties in the model, and instead announces position and collison updates over the
 * object's ids.
 * 
 * \param boundaryX The x-axis bounds of the game space.
 * \param boundaryY The y-axis bounds of the game space.
 */
PhysicsEngine::PhysicsEngine(unsigned int boundaryX, unsigned int boundaryY)
	: boundaryX(boundaryX), boundaryY(boundaryY) {}
PhysicsEngine::~PhysicsEngine() {}

/**
 * Creates a <PhysicsObject> to be managed by the <PhysicsEngine>.
 * 
 * \param id The unique id of the game object in the <GameModel>.
 * \param x The object's initial position on the x-axis.
 * \param y The object's initial position on the y-axis.
 * \param direction The object's initial direction it should move towards.
 * \param acceleration The object's initial acceleration.
 * \param deacceleration The object's initial deacceleration.
 * \param startingSpeed The object's initial speed.
 * \param maxSpeed The object's maximum allowed speed.
 * \param hitboxRadius The radius of the circle hitbox centered around the object.
 * \param accelerationType Determines the physical properties of the object's acceleration.
 */
PhysicsEngine::PhysicsObject::PhysicsObject(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius, AccelerationType accelerationType)
	: id(id), x(x), y(y), direction(direction), deltaVelocity(acceleration), deacceleration(deacceleration), maxSpeed(maxSpeed), currentSpeed(startingSpeed), velocityX(startingSpeed * sin(direction)), velocityY(startingSpeed * cos(direction)), rotation(0.0f), 
	collisionRadius(hitboxRadius), accelerationType(accelerationType), boundInWindowX(true), boundInWindowY(true)
{}
PhysicsEngine::PhysicsObject::~PhysicsObject() {}

/**
 * Adds a physical representation of an actor to be managed in the PhysicsEngine.
 * 
 * @see <PhysicsObject>
 * 
 * \param id The unique id of the game object in the <GameModel>.
 * \param x The object's initial position on the x-axis.
 * \param y The object's initial position on the y-axis.
 * \param direction The object's initial direction it should move towards.
 * \param acceleration The object's initial acceleration.
 * \param deacceleration The object's initial deacceleration.
 * \param startingSpeed The object's initial speed.
 * \param maxSpeed The object's maximum allowed speed.
 * \param hitboxRadius The radius of the circle hitbox centered around the object.
 * \param hasHitboxRegistration
 * \param accelerationType Determines the physical properties of the object's acceleration.
 */
void PhysicsEngine::addActor(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius, bool hasHitboxRegistration, AccelerationType accelerationType) {
	std::shared_ptr<PhysicsObject> newActor = std::make_shared<PhysicsObject>(id, x, y, direction, acceleration, deacceleration, startingSpeed, maxSpeed, hitboxRadius, accelerationType);
	this->actorPhysicsObjects.emplace_back(newActor);
	if (hasHitboxRegistration) {
		this->actorsWithHitboxRegistration.emplace_back(newActor);
	}
}

//TODO: Remove
/**
 * Adds a physical representation of the player to be managed in the PhysicsEngine.
 * 
 * @see PhysicsEngine::addActor()
 * 
 * \param id
 * \param x
 * \param y
 * \param direction
 * \param acceleration
 * \param deacceleration
 * \param startingSpeed
 * \param maxSpeed
 * \param hitboxRadius
 */
void PhysicsEngine::addPlayer(unsigned int id, float x, float y, float direction, float acceleration, float deacceleration, float startingSpeed, float maxSpeed, float hitboxRadius) {
	this->player = std::make_shared<PhysicsObject>(id, x, y, direction, acceleration, deacceleration, startingSpeed, maxSpeed, hitboxRadius, AccelerationType::Linear);
	this->actorPhysicsObjects.emplace_back(this->player);
	this->actorsWithHitboxRegistration.emplace_back(this->player);
}

/**
 * Removes the <PhysicsObject> of a game object.
 * 
 * \param id The id of the game object.
 */
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

/**
 * Updates the position of every <PhysicsObject> for one tick.
 * 
 * \return Returns a list containing the id and new x,y,z coordinates of the game object.
 */
std::vector <std::tuple<unsigned int, float, float, float>> PhysicsEngine::updatePositions() {

	std::vector <std::tuple<unsigned int, float, float, float>> calculatedPositions;

	for (auto &actor : this->actorPhysicsObjects) {
		actor->currentSpeed = sqrt(pow(actor->velocityX, 2.0f) + pow(actor->velocityY, 2.0f));

		if (actor->accelerationType == AccelerationType::Linear) {
			if (actor->deltaVelocity == 0.0f && actor->currentSpeed < 0.1f) {
				actor->currentSpeed = 0.0f;
				actor->velocityX = 0.0f;
				actor->velocityY = 0.0f;
			} else {
				// deacceleration
				if (actor->currentSpeed > 0.0f && actor->deacceleration != 0.0f) {
					float velocityXNormalized = actor->velocityX / actor->currentSpeed;
					float velocityYNormalized = actor->velocityY / actor->currentSpeed;

					actor->deacceleration = actor->currentSpeed * 0.005f;
					actor->currentSpeed = std::max(0.0f, actor->currentSpeed - (actor->deacceleration));

					actor->velocityX = actor->currentSpeed * velocityXNormalized;
					actor->velocityY = actor->currentSpeed * velocityYNormalized;
				}

				// acceleration
				actor->velocityX += actor->deltaVelocity * sin(actor->direction);
				actor->velocityY += actor->deltaVelocity * cos(actor->direction);

				
				// max speed
				if (actor->currentSpeed > 0.0f) {
					float velocityXNormalized = actor->velocityX / actor->currentSpeed;
					float velocityYNormalized = actor->velocityY / actor->currentSpeed;

					actor->currentSpeed = std::min(actor->maxSpeed, sqrt(pow(actor->velocityX, 2.0f) + pow(actor->velocityY, 2.0f)));

					actor->velocityX = actor->currentSpeed * velocityXNormalized;
					actor->velocityY = actor->currentSpeed * velocityYNormalized;
				}
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
		if (actor->boundInWindowX) {
			if (actor->x > this->boundaryX) {
				actor->x = fmod(actor->x, this->boundaryX);
			}
			else if (actor->x < 0.0f) {
				actor->x = this->boundaryX - fmod(actor->x, this->boundaryX);
			}
		}

		if (actor->boundInWindowY) {
			if (actor->y > this->boundaryY) {
				actor->y = fmod(actor->y, this->boundaryY);
			}
			else if (actor->y < 0.0f) {
				actor->y = this->boundaryY - fmod(actor->y, this->boundaryY);
			}
		}
		
		std::tuple<unsigned int, float, float, float> actorPositions {actor->id, actor->x, actor->y, actor->rotation};

		calculatedPositions.push_back(actorPositions);
	}

	return calculatedPositions;
}

/**
 * Sets the direction of a <PhysicsObject>.
 * 
 * \param id The id of the game object the <PhysicsObject> belongs to.
 * \param directionInRad The new direction of the <PhysicsObject> with a radian value.
 */
void PhysicsEngine::setDirection(unsigned int id, float directionInRad) {
	auto it = std::find_if(this->actorPhysicsObjects.begin(), this->actorPhysicsObjects.end(), [id](std::shared_ptr<PhysicsObject> object) { return object->id == id; });

	if (it != this->actorPhysicsObjects.end()) {
		it->get()->direction = directionInRad;
	}
}

/**
 * Sets the acceleration of a <PhysicsObject>.
 * 
 * \param id The id of the <PhysicsObject> the game object belongs to.
 * \param acceleration The new acceleration of the <PhysicsObject>.
 */
void PhysicsEngine::setAcceleration(unsigned int id, float acceleration) {
	auto it = std::find_if(this->actorPhysicsObjects.begin(), this->actorPhysicsObjects.end(), [id](std::shared_ptr<PhysicsObject> object) { return object->id == id; });

	if (it != this->actorPhysicsObjects.end()) {
		it->get()->deltaVelocity = acceleration;
	}
}

/**
 * Rotates the player by DELTA to the left in the next tick.
 */
void PhysicsEngine::rotatePlayerLeft() {
	this->player->direction -= DELTA;
	if (this->player->direction <= 0.0f) {
		this->player->direction = 2.0f * MY_PI;
	}

	this->player->rotation = this->player->direction;
}

/**
 * Rotates the player by DELTA to the right in the next tick.
 */
void PhysicsEngine::rotatePlayerRight() {
	this->player->direction += DELTA;
	if (this->player->direction >= (2 * MY_PI)) {
		this->player->direction = 0.0f;
	}

	this->player->rotation = this->player->direction;
}

/**
 * Checks the collision of every <PhysicsObject> where hitbox registration is enabled.
 * 
 * \return Returns a list of id pairs belonging to the objects that have collided.
 */
std::vector<std::pair<unsigned int, unsigned int>> PhysicsEngine::checkCollisions() {
	std::vector<std::pair<unsigned int, unsigned int>> test;

	for (auto& actor : this->actorPhysicsObjects) {
		if (actor->id == this->player->id) {
			continue;
		}

		for (auto& actorWithHitboxRegistration : this->actorsWithHitboxRegistration) {
			if (actor->id != actorWithHitboxRegistration->id) {
				auto dx = actorWithHitboxRegistration->x - actor->x;
				auto dy = actorWithHitboxRegistration->y - actor->y;
				auto distance = sqrt(dx * dx + dy * dy);

				if (distance < (actorWithHitboxRegistration->collisionRadius + actor->collisionRadius)) {
					std::pair<unsigned int, unsigned int> collisionIds = { actorWithHitboxRegistration->id, actor->id };
					test.push_back(collisionIds);
				}
			}
		}
	}
	
	return test;
}

/**
 * Sets which game space boundaries a <PhysicsObject> will wrap around.
 * 
 * \param id The id of the game object the <PhysicsObject> belongs to
 * \param x A bool whether the game object wraps around the vertical bounds.
 * \param y A bool whether the game object wraps around the horizontal bounds.
 */
void PhysicsEngine::setBoundByWindow(unsigned int id, bool x, bool y) {
	auto it = std::find_if(this->actorPhysicsObjects.begin(), this->actorPhysicsObjects.end(), [id](std::shared_ptr<PhysicsObject> object) { return object->id == id; });

	if (it != this->actorPhysicsObjects.end()) {
		it->get()->boundInWindowX = x;
		it->get()->boundInWindowY = y;
	}
}
