#include "GameModel.h"

GameModel::GameModel(unsigned int windowX, unsigned int windowY) 
	: windowX(windowX), windowY(windowY), physicsEngine(windowX, windowY), projectileCooldown(std::chrono::steady_clock::now() - std::chrono::seconds(1)) {
}
GameModel::~GameModel() {
}

void GameModel::setCurrentTime() {
	this->currentTime = steady_clock::now();
}

void GameModel::AddPlayer(float startingPosition[3], float rotation) {
	std::shared_ptr<PlayerModel> playerModel = std::make_shared<PlayerModel>(startingPosition, rotation);
	this->actors.insert({playerModel->id, playerModel});
	this->player = playerModel;

	this->physicsEngine.addPlayer(playerModel->id, playerModel->position[0], playerModel->position[1], 0.0f, 0.0f, 0.005f, 0.0f, 4.0f, playerModel->radius);
}

float getRandomFloat(float min, float max)
{
	float random = ((float)rand()) / (float)RAND_MAX;

	// generate (in your case) a float between 0 and (4.5-.78)
	// then add .78, giving you a float between .78 and 4.5
	float range = max - min;
	return (random * range) + min;
}

void GameModel::AddAsteroid(float startingPosition[3]) {
	std::shared_ptr<AsteroidModel> asteroidModel = std::make_shared<AsteroidModel>(startingPosition);
	this->actors.insert({asteroidModel->id, asteroidModel});
	this->asteroids.push_back(asteroidModel);

	this->physicsEngine.addActor(asteroidModel->id, asteroidModel->position[0], asteroidModel->position[1], getRandomFloat(0.0f, 2 * MY_PI), 0.0f, 0.0f, 1.5f, 1.5f, asteroidModel->radius, false, AccelerationType::Linear);
}

void GameModel::addMediumAsteroid(float startingPosition[3]) {
	std::shared_ptr<MediumAsteroidModel> asteroidModel = std::make_shared<MediumAsteroidModel>(startingPosition);
	this->actors.insert({ asteroidModel->id, asteroidModel });
	this->mediumAsteroids.push_back(asteroidModel);

	this->physicsEngine.addActor(asteroidModel->id, asteroidModel->position[0], asteroidModel->position[1], getRandomFloat(0.0f, 2 * MY_PI), 0.0f, 0.0f, 1.5f, 1.5f, asteroidModel->radius, false, AccelerationType::Linear);
}

void GameModel::addSmallAsteroid(float startingPosition[3]) {
	std::shared_ptr<SmallAsteroidModel> asteroidModel = std::make_shared<SmallAsteroidModel>(startingPosition);
	this->actors.insert({ asteroidModel->id, asteroidModel });
	this->smallAsteroids.push_back(asteroidModel);

	this->physicsEngine.addActor(asteroidModel->id, asteroidModel->position[0], asteroidModel->position[1], getRandomFloat(0.0f, 2 * MY_PI), 0.0f, 0.0f, 1.5f, 1.5f, asteroidModel->radius, false, AccelerationType::Linear);
}

void GameModel::addLargeShip() {
	auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());

	bool startOnLeft = gen();
	float x; 
	float y;
	float direction;

	if (startOnLeft) {
		x = 0.0f;
		y = getRandomFloat(0.0f, windowY);
		direction = MY_PI;
	} else {
		x = windowX;
		y = getRandomFloat(0.0f, windowY);
		direction = 0.0f;
	}

	float startingPosition[3] = { x, y, 0.0f };

	std::shared_ptr<LargeShipModel> shipModel = std::make_shared<LargeShipModel>(startingPosition, this->currentTime, startOnLeft);

	this->actors.insert({ shipModel->id, shipModel });
	this->ship = shipModel;

	this->physicsEngine.addActor(shipModel->id, shipModel->position[0], shipModel->position[1], direction, 0.0f, 0.0f, 1.0f, 1.0f, shipModel->radius, false, AccelerationType::None);
}


void GameModel::setPlayerAccelerating(bool isAccelerating) {
	if (!this->player) {
		return;
	}

	if (isAccelerating) {
		this->physicsEngine.setAcceleration(this->player->id, 0.05f);
	}
	else {
		this->physicsEngine.setAcceleration(this->player->id, 0.0f);
	}
}

void GameModel::fireProjectile() {
	if (!this->player) {
		return;
	}

	duration<double> timeSpan = duration_cast<duration<double>>(currentTime - this->projectileCooldown);
	if (timeSpan.count() > 0.25 && this->projectiles.size() < 4) {
		this->projectileCooldown = currentTime;

		std::shared_ptr<ProjectileModel> projectileModel = std::make_shared<ProjectileModel>(this->player->position, currentTime);
		this->actors.insert({ projectileModel->id, projectileModel });
		this->projectiles.push_back(projectileModel);

		this->physicsEngine.addActor(projectileModel->id, projectileModel->position[0], projectileModel->position[1], this->player->rotation, 0.000f, 1.0f, 6.0f, 6.0f, projectileModel->radius, true, AccelerationType::Linear);
	}
}

void GameModel::removeActor(unsigned int id) {
	auto actor = this->actors.find(id);
	
	if (actor != this->actors.end()) {
		if (actor->second->type == "projectile") {
			auto it = std::find_if(this->projectiles.begin(), this->projectiles.end(), [id](std::shared_ptr<ProjectileModel> object) { return object->id == id; });

			if (it != this->projectiles.end()) {
				this->projectiles.erase(it);
			}
		} else if (actor->second->type == "asteroid") {
			auto it = std::find_if(this->asteroids.begin(), this->asteroids.end(), [id](std::shared_ptr<AsteroidModel> object) { return object->id == id; });

			if (it != this->asteroids.end()) {
				float startingPosition[3]{ it->get()->position[0], it->get()->position[1], it->get()->position[2] };
				this->asteroids.erase(it);
				this->addMediumAsteroid(startingPosition);
				this->addMediumAsteroid(startingPosition);
			}
		} else if (actor->second->type == "mediumAsteroid") {
			auto it = std::find_if(this->mediumAsteroids.begin(), this->mediumAsteroids.end(), [id](std::shared_ptr<MediumAsteroidModel> object) { return object->id == id; });

			if (it != this->mediumAsteroids.end()) {
				float startingPosition[3]{ it->get()->position[0], it->get()->position[1], it->get()->position[2] };
				this->mediumAsteroids.erase(it);
				this->addSmallAsteroid(startingPosition);
				this->addSmallAsteroid(startingPosition);
			}
		} else if (actor->second->type == "smallAsteroid") {
			auto it = std::find_if(this->smallAsteroids.begin(), this->smallAsteroids.end(), [id](std::shared_ptr<SmallAsteroidModel> object) { return object->id == id; });
			if (it != this->smallAsteroids.end()) {
				this->smallAsteroids.erase(it);
			}
		} else if (actor->second->type == "ship") {
			this->ship.reset();
		} else if (actor->second->type == "player") {
			this->player.reset();
			this->lastPlayerDeath = this->currentTime;
		}

		this->physicsEngine.removeActor(id);
		this->actors.erase(id);
	}
	else {
		std::cout << "Could not remove actor with id " << id << "." << std::endl;
	}
}

void GameModel::RotatePlayerLeft() {
	//player->RotateLeft();
	this->physicsEngine.rotatePlayerLeft();
}

void GameModel::RotatePlayerRight() {
	//player->RotateRight();
	this->physicsEngine.rotatePlayerRight();
}

void GameModel::Setup() {
	this->setCurrentTime();

	float initialPlayerPosition[3] = { 200.0f, 200.0f, 0.0f };
	this->AddPlayer(initialPlayerPosition, 0.0f);

	for (int i = 0; i < 5; i++) {
		float initialAsteroidPosition[3] = { rand() % 700, rand() % 300, 0.0f };
		this->AddAsteroid(initialAsteroidPosition);
	}

	this->addLargeShip();
}

void GameModel::checkPlayerDeath() {
	if (this->player) {
		return;
	}

	duration<double> timeSpan = duration_cast<duration<double>>(this->currentTime - this->lastPlayerDeath);

	if (timeSpan.count() > 3.0) {
		float startingPosition[3] = { this->windowX/2.0f, this->windowY/2.0f, 0.0f };
		this->AddPlayer(startingPosition, 0.0f);
	}
}

void GameModel::updatePositions() {
	//TODO: return rotations too
	auto test = this->physicsEngine.updatePositions();

	for (auto& testt : test) {
		//actor->SetPosition(this->physicsEngine->getPosition(actor->id));
		auto peter = this->actors.find(std::get<0>(testt));

		if (peter != this->actors.end()) {
			peter->second->position[0] = std::get<1>(testt);
			peter->second->position[1] = std::get<2>(testt);
			peter->second->rotation = std::get<3>(testt);

		}
	}
}

void GameModel::checkCollisions() {
	auto test = this->physicsEngine.checkCollisions();

	for (auto& collisionPairIds : test) {
		if (this->actors.count(collisionPairIds.first) == 1 && this->actors.count(collisionPairIds.second) == 1) {
			if (this->player != nullptr) { //keep checking collisions while player is dead but avoid nullptr exception
				if ((this->actors[collisionPairIds.first]->id == this->player->id && this->actors[collisionPairIds.second]->type == "projectile") ||
					(this->actors[collisionPairIds.second]->id == this->player->id && this->actors[collisionPairIds.first]->type == "projectile")) {
					continue;
				}
			}


			this->actors[collisionPairIds.first]->hasBeenHit();
			this->actors[collisionPairIds.second]->hasBeenHit();

			this->removeActor(collisionPairIds.first);
			this->removeActor(collisionPairIds.second);
		}
	}
}

void GameModel::checkProjectileLifetimes() {
	std::vector<unsigned int> idsToRemove;
	for (auto& projectile : this->projectiles) {
		duration<double> timeSpan = duration_cast<duration<double>>(this->currentTime - projectile->timeOfSpawn);

		if (timeSpan.count() > 2.0) {
			idsToRemove.push_back(projectile->id);
		}
	}

	for (unsigned int id : idsToRemove) {
		this->removeActor(id);
	}
}

void GameModel::setShipDirection() {
	if (!this->ship) {
		return;
	}

	duration<double> timeSpan = duration_cast<duration<double>>(currentTime - this->ship->lastChangeOfDirection);
	if (timeSpan.count() >= 2.0) {
		MovingState newState = this->ship->changeDirection();

		float newDirectionInRad;

		switch (newState) {
		case MovingState::Left:
			newDirectionInRad = MY_PI;
			break;
		case MovingState::LeftUp:
			newDirectionInRad = (2 * MY_PI) / 3;
			break;
		case MovingState::LeftDown:
			newDirectionInRad = (MY_PI / 3) * 4;
			break;
		case MovingState::Right:
			newDirectionInRad = 0.0f;
			break;
		case MovingState::RightUp:
			newDirectionInRad = MY_PI / 3;
			break;
		case MovingState::RightDown:
			newDirectionInRad = (MY_PI / 3) * 5;
			break;
		default:
			return;
		}

		this->physicsEngine.setDirection(this->ship->id, newDirectionInRad);
		this->ship->lastChangeOfDirection = currentTime;
	}
}