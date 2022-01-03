#pragma once

#include "ActorModel.h"
#include "PlayerModel.h"
#include "AsteroidModel.h"
#include "ProjectileModel.h"

#include "physics/Engine.h"

#include <vector>
#include <memory>
#include <map>
#include <chrono>

using namespace std::chrono;

class GameModel {
	std::shared_ptr<PlayerModel> player;
	PhysicsEngine physicsEngine;
	
	steady_clock::time_point currentTime;
	steady_clock::time_point projectileCooldown;
	steady_clock::time_point lastPlayerDeath;
public:
	unsigned int windowX;
	unsigned int windowY;
	std::vector<std::shared_ptr<AsteroidModel>> asteroids;
	std::vector<std::shared_ptr<ProjectileModel>> projectiles;
	std::map<unsigned int, std::shared_ptr<ActorModel>> actors;

	GameModel(unsigned int windowX, unsigned int windowY);
	~GameModel();
	
	void Setup();
	void setCurrentTime();

	void AddPlayer(float startingPosition[3], float rotation);
	void AddAsteroid(float startingPosition[3]);
	void removeActor(unsigned int id);

	void setPlayerAccelerating(bool isAccelerating);
	void RotatePlayerRight();
	void RotatePlayerLeft();
	void fireProjectile();

	void updatePositions();
	void checkCollisions();
	void checkProjectileLifetimes();

	void checkPlayerDeath();
};