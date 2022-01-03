#include "GameModel.h"

GameModel::GameModel(unsigned int windowX, unsigned int windowY) : windowX(windowX), windowY(windowY), physicsEngine(windowX, windowY) {
}
GameModel::~GameModel() {
}

void GameModel::AddPlayer(float startingPosition[3], float rotation) {
	std::shared_ptr<PlayerModel> playerModel = std::make_shared<PlayerModel>(startingPosition, rotation);
	this->actors.insert({playerModel->id, playerModel});
	this->player = playerModel;

	this->physicsEngine.addPlayer(playerModel->id, playerModel->position[0], playerModel->position[1], 0.0f, 0.0f, playerModel->radius);
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

	this->physicsEngine.addActor(asteroidModel->id, asteroidModel->position[0], asteroidModel->position[1], getRandomFloat(0.0f, 2 * MY_PI), 0.002f, asteroidModel->radius);
}

void GameModel::setPlayerAccelerating(bool isAccelerating) {
	this->physicsEngine.setPlayerAccelerating(isAccelerating);
}

void GameModel::fireProjectile() {
	std::shared_ptr<ProjectileModel> projectileModel = std::make_shared<ProjectileModel>(this->player->position);
	this->actors.insert({ projectileModel->id, projectileModel });

	this->physicsEngine.addActor(projectileModel->id, projectileModel->position[0], projectileModel->position[1], this->player->rotation, 0.006f, projectileModel->radius);
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
	float initialPlayerPosition[3] = { 0.0f, 0.0f, 0.0f };
	this->AddPlayer(initialPlayerPosition, 0.0f);

	for (int i = 0; i < 5; i++) {
		float initialAsteroidPosition[3] = { rand() % 700, rand() % 300, 0.0f };
		this->AddAsteroid(initialAsteroidPosition);
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
		/*if (this->actors.count(collisionPairIds.first) == 1) {
			//this->actors[collisionPairIds.first]->hasBeenHit();
			//this->actors.erase(collisionPairIds.first);
			//this->physicsEngine.removeActor(collisionPairIds.first);
		}

		if (this->actors.count(collisionPairIds.second) == 1) {
			//this->actors[collisionPairIds.second]->hasBeenHit();

			this->actors.erase(collisionPairIds.second);
			this->physicsEngine.removeActor(collisionPairIds.second);
		}*/
	}
}