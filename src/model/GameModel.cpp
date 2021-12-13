#include "GameModel.h"

GameModel::GameModel(unsigned int windowX, unsigned int windowY) : windowX(windowX), windowY(windowY) {
}
GameModel::~GameModel() {
}

void GameModel::AddPlayer(float startingPosition[3], float rotation) {
	std::shared_ptr<PlayerModel> playerModel = std::make_shared<PlayerModel>(startingPosition, rotation, this->windowX, this->windowY);
	this->actors.insert({playerModel->id, playerModel});
	this->player = playerModel;
}

void GameModel::AddAsteroid(float startingPosition[3]) {
	std::shared_ptr<AsteroidModel> asteroidModel = std::make_shared<AsteroidModel>(startingPosition, this->windowX, this->windowY);
	this->actors.insert({asteroidModel->id, asteroidModel});
	this->asteroids.push_back(asteroidModel);
}

void GameModel::MovePlayerForward() {
	player->MoveForward();
}

void GameModel::RotatePlayerLeft() {
	player->RotateLeft();
}

void GameModel::RotatePlayerRight() {
	player->RotateRight();
}

void GameModel::Setup() {
	float initialPlayerPosition[3] = { 0.0f, 0.0f, 0.0f };
	this->AddPlayer(initialPlayerPosition, 0.0f);

	for (int i = 0; i < 5; i++) {
		float initialAsteroidPosition[3] = { rand() % 700, rand() % 300, 0.0f };
		this->AddAsteroid(initialAsteroidPosition);
	}
}