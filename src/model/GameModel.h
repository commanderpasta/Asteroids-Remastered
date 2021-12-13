#pragma once

#include "ActorModel.h"
#include "PlayerModel.h"
#include "AsteroidModel.h"

#include <vector>
#include <memory>
#include <map>

class GameModel {
	std::shared_ptr<PlayerModel> player;
public:
	unsigned int windowX;
	unsigned int windowY;
	std::vector<std::shared_ptr<AsteroidModel>> asteroids;
	std::map<unsigned int, std::shared_ptr<ActorModel>> actors;

	GameModel(unsigned int windowX, unsigned int windowY);
	~GameModel();
	
	void Setup();

	void AddPlayer(float startingPosition[3], float rotation);
	void AddAsteroid(float startingPosition[3]);

	void MovePlayerForward();
	void RotatePlayerRight();
	void RotatePlayerLeft();
};