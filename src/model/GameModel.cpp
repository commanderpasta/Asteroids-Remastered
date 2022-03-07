#include "GameModel.h"

GameModel::GameModel(unsigned int windowX, unsigned int windowY)
	: windowX(windowX), currentFrameTime(steady_clock::now()), playerIsInHyperSpace(false), windowY(windowY), physicsEngine(windowX, windowY), score(0), ticksPassed(0), playerLives(3), pointsUntilExtraLive(10000) {
	this->lastFrameTime = this->currentFrameTime;
	this->lastHyperSpaceActivation = this->currentFrameTime;
	this->lastBackgroundSound = this->currentFrameTime;
	this->levelSystem.setBeginTime(currentFrameTime);

	this->soundFileNames = {"background1", "background2", "booster", "extralife", "projectile", "ship1", "ship2", "ship3", "explosion1"};
}
GameModel::~GameModel() {
}

void GameModel::Setup() {
	float initialPlayerPosition[3] = { 200.0f, 200.0f, 0.0f };
	this->AddPlayer(initialPlayerPosition, 0.0f);
}

void GameModel::setCurrentTime() {
	this->lastFrameTime = this->currentFrameTime;
	this->currentFrameTime = steady_clock::now();

	//double timeSpan = duration_cast<duration<double>>(this->currentFrameTime - this->lastFrameTime).count();
	//this->ticksPassed = timeSpan * TICK_RATE;
}

void GameModel::checkLevel() {
	if (this->levelSystem.isLevelActive) {
		if (this->asteroids.size() == 0 && this->mediumAsteroids.size() == 0 && this->smallAsteroids.size() == 0) {
			this->levelSystem.nextLevel(this->currentFrameTime);
			this->soundEvents.push_back({ SoundAction::STOP, "background1" });
			this->soundEvents.push_back({ SoundAction::STOP, "background2" });
		}
		else {
			if (this->levelSystem.canShipSpawn(this->currentFrameTime)) {
				this->addShip(rand() % 2);
			}

			float backgroundSoundFrequency = 0.5 - 0.06 * min(3, this->levelSystem.getAmountOfAsteroidSpawns() - this->asteroids.size());
			duration<double> timeSpan = duration_cast<duration<double>>(this->currentFrameTime - this->lastBackgroundSound);

			if (timeSpan.count() > backgroundSoundFrequency && (this->player || this->playerIsInHyperSpace)) {
				if (this->backgroundSoundSwitch) {
					this->soundEvents.push_back({ SoundAction::PLAY, "background1" });
				}
				else {
					this->soundEvents.push_back({ SoundAction::PLAY, "background2" });
				}
				this->backgroundSoundSwitch = !this->backgroundSoundSwitch;
				this->lastBackgroundSound = this->currentFrameTime;
			}
		}
	}
	else {
		if (this->levelSystem.canStartLevel(currentFrameTime)) {
			unsigned int spawnAsteroidCount = this->levelSystem.getAmountOfAsteroidSpawns();

			for (unsigned int i = 0; i < spawnAsteroidCount; i++) {
				float initialAsteroidPosition[3] = { getRandomFloat(0.0f, windowX), getRandomFloat(0.0f, windowY), 0.0f };
				this->AddAsteroid(initialAsteroidPosition);
			}
		}
	}

}

void GameModel::AddPlayer(float startingPosition[3], float rotation) {
	std::shared_ptr<PlayerModel> playerModel = std::make_shared<PlayerModel>(startingPosition, rotation);
	this->actors.insert({playerModel->id, playerModel});
	this->player = playerModel;

	this->physicsEngine.addPlayer(playerModel->id, playerModel->position[0], playerModel->position[1], 0.0f, 0.0f, 0.005f, 0.0f, 4.0f, playerModel->radius);
}

void GameModel::AddAsteroid(float startingPosition[3]) {
	std::shared_ptr<AsteroidModel> asteroidModel = std::make_shared<AsteroidModel>(startingPosition);
	this->actors.insert({asteroidModel->id, asteroidModel});
	this->asteroids.push_back(asteroidModel);

	this->physicsEngine.addActor(asteroidModel->id, asteroidModel->position[0], asteroidModel->position[1], getRandomFloat(0.0f, 2 * MY_PI), 0.0f, 0.0f, 0.5f, 0.5f, asteroidModel->radius, false, AccelerationType::Linear);
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

void GameModel::spawnDeathParticles(float sourcePosition[3], ActorType sourceType) {
	for (int i = 0; i < 8; i++) {
		std::shared_ptr<ParticleModel> particleModel = std::make_shared<ParticleModel>(this->currentFrameTime, sourcePosition, sourceType);
		this->actors.insert({ particleModel->id, particleModel });
		this->particles.push_back(particleModel);

		float randomSpeed = getRandomFloat(0.1f, 0.25f);
		float randomDirection = getRandomFloat(0.0f, 2 * MY_PI);
		this->physicsEngine.addActor(particleModel->id, particleModel->position[0], particleModel->position[1], randomDirection, 0.0f, 0.0f, randomSpeed, randomSpeed, 0.0f, false, AccelerationType::Linear);
	}
}

void GameModel::setPlayerAccelerating(bool isAccelerating) {
	if (!this->player) {
		return;
	}

	if (isAccelerating) {
		this->physicsEngine.setAcceleration(this->player->id, 0.015f);
		this->soundEvents.push_back({ SoundAction::LOOP, "booster" });
		this->player->isAccelerating = true;
	}
	else {
		this->physicsEngine.setAcceleration(this->player->id, 0.0f);
		this->soundEvents.push_back({ SoundAction::STOP, "booster" });
		this->player->isAccelerating = false;
	}

	this->player->checkBoosterActive(this->currentFrameTime);
}

void GameModel::shipFireProjectile() {
	if (!this->ship || !this->player) {
		return;
	}

	duration<double> timeSpan = duration_cast<duration<double>>(currentFrameTime - this->ship->projectileCooldown);
	if (timeSpan.count() > 0.8 && this->ship->activeProjectileCount < 2) {
		this->ship->projectileCooldown = currentFrameTime;
		this->ship->activeProjectileCount++;

		std::shared_ptr<ProjectileModel> projectileModel = std::make_shared<ProjectileModel>(ship->position, currentFrameTime, ship->id);
		this->actors.insert({ projectileModel->id, projectileModel });
		this->projectiles.push_back(projectileModel);

		// calculate direction (in rad) ship to the player
		float direction = this->ship->calcProjectileDirection(this->player->position);
		this->physicsEngine.addActor(projectileModel->id, projectileModel->position[0], projectileModel->position[1], direction, 0.000f, 0.0f, 3.0f, 3.0f, projectileModel->radius, true, AccelerationType::Linear);
	}
}


void GameModel::playerFireProjectile() {
	if (!this->player) {
		return;
	}

	duration<double> timeSpan = duration_cast<duration<double>>(this->currentFrameTime - this->player->projectileCooldown);
	if (timeSpan.count() > 0.25 && this->player->activeProjectileCount < 4) {
		this->player->projectileCooldown = this->currentFrameTime;
		this->player->activeProjectileCount++;

		std::shared_ptr<ProjectileModel> projectileModel = std::make_shared<ProjectileModel>(this->player->position, this->currentFrameTime, this->player->id);
		this->actors.insert({ projectileModel->id, projectileModel });
		this->projectiles.push_back(projectileModel);

		this->physicsEngine.addActor(projectileModel->id, projectileModel->position[0], projectileModel->position[1], this->player->rotation, 0.000f, 1.0f, 6.0f, 6.0f, projectileModel->radius, true, AccelerationType::Linear);
		this->soundEvents.push_back({SoundAction::PLAY, "projectile"});
	}
}

void GameModel::removeActor(unsigned int id) {
	auto actor = this->actors.find(id);
	
	if (actor != this->actors.end()) {
		if (actor->second->actorType == ActorType::Projectile) {
			auto it = std::find_if(this->projectiles.begin(), this->projectiles.end(), [id](std::shared_ptr<ProjectileModel> object) { return object->id == id; });

			if (it != this->projectiles.end()) {
				if (this->player && this->player->id == it->get()->ownerId) {
					this->player->activeProjectileCount--;
				} else if (this->ship && this->ship->id == it->get()->ownerId) {
					this->ship->activeProjectileCount--;
				}

				this->projectiles.erase(it);
			}
		} else if (actor->second->actorType == ActorType::AsteroidLarge) {
			auto it = std::find_if(this->asteroids.begin(), this->asteroids.end(), [id](std::shared_ptr<AsteroidModel> object) { return object->id == id; });

			if (it != this->asteroids.end()) {
				float startingPosition[3]{ it->get()->position[0], it->get()->position[1], it->get()->position[2] };
				this->spawnDeathParticles(startingPosition, it->get()->actorType);

				this->asteroids.erase(it);
				this->addMediumAsteroid(startingPosition);
				this->addMediumAsteroid(startingPosition);
				this->soundEvents.push_back({ SoundAction::PLAY, "explosion1" });
			}
		} else if (actor->second->actorType == ActorType::AsteroidMedium) {
			auto it = std::find_if(this->mediumAsteroids.begin(), this->mediumAsteroids.end(), [id](std::shared_ptr<MediumAsteroidModel> object) { return object->id == id; });

			if (it != this->mediumAsteroids.end()) {
				float startingPosition[3]{ it->get()->position[0], it->get()->position[1], it->get()->position[2] };
				this->spawnDeathParticles(startingPosition, it->get()->actorType);

				this->mediumAsteroids.erase(it);
				this->addSmallAsteroid(startingPosition);
				this->addSmallAsteroid(startingPosition);
				this->soundEvents.push_back({ SoundAction::PLAY, "explosion1" });
			}
		} else if (actor->second->actorType == ActorType::AsteroidSmall) {
			auto it = std::find_if(this->smallAsteroids.begin(), this->smallAsteroids.end(), [id](std::shared_ptr<SmallAsteroidModel> object) { return object->id == id; });

			if (it != this->smallAsteroids.end()) {
				this->spawnDeathParticles(it->get()->position, it->get()->actorType);

				this->smallAsteroids.erase(it);
				this->soundEvents.push_back({ SoundAction::PLAY, "explosion1" });
			}

		} else if (actor->second->actorType == ActorType::ShipLarge) {
			if (!this->ship->hasReachedOtherSide(this->windowX)) {
				this->soundEvents.push_back({ SoundAction::PLAY, "explosion1" });
				this->spawnDeathParticles(this->ship->position, this->ship->actorType);
			}
			this->ship.reset();
			this->soundEvents.push_back({ SoundAction::STOP, "ship3" });
			this->soundEvents.push_back({ SoundAction::STOP, "ship2" });
		} else if (actor->second->actorType == ActorType::ShipSmall) {
			if (!this->ship->hasReachedOtherSide(this->windowX)) {
				this->soundEvents.push_back({ SoundAction::PLAY, "explosion1" });
				this->spawnDeathParticles(this->ship->position, this->ship->actorType);
			}
			this->ship.reset();
			this->soundEvents.push_back({ SoundAction::STOP, "ship1" });
			this->soundEvents.push_back({ SoundAction::STOP, "ship2" });
		} else if (actor->second->actorType == ActorType::Player) {
			if (!this->playerIsInHyperSpace) {
				this->playerLives--;
				this->lastPlayerDeath = this->currentFrameTime;
				this->soundEvents.push_back({ SoundAction::STOP, "booster" });
				this->soundEvents.push_back({ SoundAction::PLAY, "explosion1" });
				this->spawnDeathParticles(this->player->position, this->player->actorType);
			}

			this->player.reset();
		} else if (actor->second->actorType == ActorType::Particle || actor->second->actorType == ActorType::PlayerParticle) {
			auto it = std::find_if(this->particles.begin(), this->particles.end(), [id](std::shared_ptr<ParticleModel> object) { return object->id == id; });
			
			if (it != this->particles.end()) {
				this->particles.erase(it);
			}
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

void GameModel::checkPlayerDeath() {
	if (this->player || this->playerIsInHyperSpace) {
		return;
	}

	duration<double> timeSpan = duration_cast<duration<double>>(this->currentFrameTime - this->lastPlayerDeath);

	if (timeSpan.count() > 3.0) {
		float startingPosition[3] = { this->windowX/2.0f, this->windowY/2.0f, 0.0f };
		this->AddPlayer(startingPosition, 0.0f);
	}
}

void GameModel::checkPlayerHyperSpace() {
	if (!this->playerIsInHyperSpace) {
		return;
	}

	duration<double> timeSpan = duration_cast<duration<double>>(this->currentFrameTime - this->lastHyperSpaceActivation);

	if (timeSpan.count() > 2.0) {
		//avoid spawning player on edge of map
		float boundaryX = this->windowX * 0.9f;
		float boundaryY = this->windowY * 0.9f;

		float startingPosition[3] = { getRandomFloat(this->windowX - boundaryX, boundaryX), getRandomFloat(this->windowY - boundaryY, boundaryY), 0.0f};
		this->AddPlayer(startingPosition, getRandomFloat(0.0f, 2 * MY_PI));
		this->playerIsInHyperSpace = false;
	}
}

void GameModel::activateHyperSpace() {
	if (!this->player) {
		return;
	}

	duration<double> timeSpan = duration_cast<duration<double>>(this->currentFrameTime - this->lastHyperSpaceActivation);

	if (timeSpan.count() > 3.0) {
		this->playerIsInHyperSpace = true;
		this->removeActor(this->player->id);
		this->lastHyperSpaceActivation = this->currentFrameTime;
	}
}


void GameModel::updatePositions() {
	duration<float> timePassed = duration_cast<duration<float>>(this->currentFrameTime - this->lastFrameTime);
	auto newPositionsById = this->physicsEngine.updatePositions(this->ticksPassed);

	for (auto& positionById : newPositionsById) {
		auto actor = this->actors.find(std::get<0>(positionById));

		if (actor != this->actors.end()) {
			actor->second->position[0] = std::get<1>(positionById);
			actor->second->position[1] = std::get<2>(positionById);
			actor->second->rotation = std::get<3>(positionById);
		}
	}
}

void GameModel::addPointsFromActor(unsigned int id) {
	auto actor = this->actors.find(id);

	if (actor != this->actors.end()) {
		unsigned int pointsToAdd = actor->second->getPointsValue();

		this->score += pointsToAdd;
		if (pointsToAdd > this->pointsUntilExtraLive || this->pointsUntilExtraLive - pointsToAdd == 0) {
			this->pointsUntilExtraLive = 10000 - (pointsToAdd - this->pointsUntilExtraLive); //carry over remainder points to next 10000
			this->playerLives++; //give an extra live
			this->soundEvents.push_back({ SoundAction::PLAY, "extralife" });
		}
	}
	else {
		std::cout << "Could not add points from actor with id " << id << "." << std::endl;
	}
}

void GameModel::checkCollisionWithProjectile(unsigned int projectileId, unsigned int targetId) {
	auto projectile = std::dynamic_pointer_cast<ProjectileModel>(this->actors[projectileId]);

	if (this->player && this->player->id == targetId && this->player->id == projectile->ownerId || this->ship && this->ship->id == targetId && this->ship->id == projectile->ownerId) {
		return;
	} else {
		this->removeActor(projectileId);
		this->addPointsFromActor(targetId);
		this->removeActor(targetId);
	}
}

void GameModel::checkCollisions() {
	auto test = this->physicsEngine.checkCollisions();

	for (auto& collisionPairIds : test) {
		if (this->actors.count(collisionPairIds.first) == 1 && this->actors.count(collisionPairIds.second) == 1) {
			if (this->actors[collisionPairIds.first]->actorType == ActorType::Particle || this->actors[collisionPairIds.second]->actorType == ActorType::Particle ||
				this->actors[collisionPairIds.first]->actorType == ActorType::PlayerParticle || this->actors[collisionPairIds.second]->actorType == ActorType::PlayerParticle) {
				continue;
			}

			if (this->player && (collisionPairIds.first == this->player->id || collisionPairIds.second == this->player->id)) {
				duration<double> timeSpan = duration_cast<duration<double>>(this->currentFrameTime - this->lastPlayerDeath);

				if (timeSpan.count() < 5.0) {
					return; //2 seconds of respawn time + 3 second invincibility on respawn after death
				}
			}

			if (this->actors[collisionPairIds.first]->actorType == ActorType::Projectile && this->actors[collisionPairIds.second]->actorType != ActorType::Projectile) {
				this->checkCollisionWithProjectile(collisionPairIds.first, collisionPairIds.second);
			}
			else if (this->actors[collisionPairIds.first]->actorType != ActorType::Projectile && this->actors[collisionPairIds.second]->actorType == ActorType::Projectile) {
				this->checkCollisionWithProjectile(collisionPairIds.second, collisionPairIds.first);
			}
			else {
				this->removeActor(collisionPairIds.first);
				this->removeActor(collisionPairIds.second);
			}
		}
	}
}

void GameModel::checkProjectileLifetimes() {
	std::vector<unsigned int> idsToRemove;
	for (auto& projectile : this->projectiles) {
		duration<double> timeSpan = duration_cast<duration<double>>(this->currentFrameTime - projectile->timeOfSpawn);

		if (timeSpan.count() > 2.0) {
			idsToRemove.push_back(projectile->id);
		}
	}

	for (unsigned int id : idsToRemove) {
		this->removeActor(id);
	}
}

void GameModel::checkParticleLifetimes() {
	std::vector<unsigned int> idsToRemove;
	for (auto& particle : this->particles) {
		if (particle->shouldDestroy(this->currentFrameTime)) {
			idsToRemove.push_back(particle->id);
		}
	}

	for (unsigned int id : idsToRemove) {
		this->removeActor(id);
	}
}

void GameModel::addShip(bool isLarge) {
	bool startOnLeft = randomBool();
	float x;
	float y;
	float direction;

	if (startOnLeft) {
		x = 0.0f;
		y = getRandomFloat(20.0f, windowY - 20.0f);
		direction = 0.0f;
	} else {
		x = windowX;
		y = getRandomFloat(20.0f, windowY - 20.0f);
		direction = MY_PI;
	}

	float startingPosition[3] = { x, y, 0.0f };

	std::shared_ptr<BaseShipModel> shipModel;

	if (isLarge) {
		shipModel = std::make_shared<LargeShipModel>(startingPosition, this->currentFrameTime, startOnLeft);
		this->soundEvents.push_back({ SoundAction::LOOP, "ship3" });
	}
	else {
		SmallShipModel smallShip(startingPosition, this->currentFrameTime, startOnLeft);

		float shootingInaccuracy = std::fmax(0.0f, 0.2f - (this->levelSystem.getCurrentLevel() * 0.05f));
		smallShip.setInaccuracy(shootingInaccuracy);

		shipModel = std::make_shared<SmallShipModel>(smallShip);
		this->soundEvents.push_back({ SoundAction::LOOP, "ship1" });
	}

	this->actors.insert({ shipModel->id, shipModel });
	this->ship = shipModel;

	this->physicsEngine.addActor(shipModel->id, shipModel->position[0], shipModel->position[1], direction, 0.0f, 0.0f, 1.0f, 0.8f, shipModel->radius, true, AccelerationType::Constant);
	this->physicsEngine.setBoundByWindow(shipModel->id, false, true);
}

void GameModel::setShipDirection() {
	if (!this->ship) {
		return;
	}

	duration<double> timeSpan = duration_cast<duration<double>>(currentFrameTime - this->ship->lastChangeOfDirection);
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
		this->ship->lastChangeOfDirection = currentFrameTime;
		this->soundEvents.push_back({ SoundAction::LOOP, "ship2" });
	}
}

void GameModel::checkShipLifetime() {
	if (this->ship && this->ship->hasReachedOtherSide(this->windowX)) {
		this->removeActor(this->ship->id);
	}
}

void GameModel::clearSoundChanges() {
	this->soundEvents.clear();
}