#pragma once

#include "Maths.h"

#include "ActorModel.h"
#include "PlayerModel.h"
#include "AsteroidModel.h"
#include "MediumAsteroidModel.h"
#include "SmallAsteroidModel.h"
#include "ProjectileModel.h"
#include "BaseShipModel.h"
#include "SmallShipModel.h"
#include "LargeShipModel.h"

#include "LevelSystem.h"
#include "physics/Engine.h"

#include <vector>
#include <memory>
#include <map>
#include <chrono>

using namespace std::chrono;

class GameModel {
	std::shared_ptr<PlayerModel> player;
	std::shared_ptr<BaseShipModel> ship;
	PhysicsEngine physicsEngine;
	LevelSystem levelSystem;
	
	steady_clock::time_point currentTime;
	steady_clock::time_point lastPlayerDeath;
public:
	unsigned int score;

	unsigned int windowX;
	unsigned int windowY;
	std::vector<std::shared_ptr<AsteroidModel>> asteroids;
	std::vector<std::shared_ptr<MediumAsteroidModel>> mediumAsteroids;
	std::vector<std::shared_ptr<SmallAsteroidModel>> smallAsteroids;
	std::vector<std::shared_ptr<ProjectileModel>> projectiles;
	std::map<unsigned int, std::shared_ptr<ActorModel>> actors;

	GameModel(unsigned int windowX, unsigned int windowY);
	~GameModel();
	
	void Setup();
	void setCurrentTime();

	void AddPlayer(float startingPosition[3], float rotation);
	void AddAsteroid(float startingPosition[3]);
	void addMediumAsteroid(float startingPosition[3]);
	void addSmallAsteroid(float startingPosition[3]);
	void addShip(bool isLarge);

	void removeActor(unsigned int id);

	void setPlayerAccelerating(bool isAccelerating);
	void RotatePlayerRight();
	void RotatePlayerLeft();
	void fireProjectile(unsigned int ownerId);
	void playerFireProjectile();

	void updatePositions();
	void checkCollisions();
	void checkCollisionWithProjectile(unsigned int projectileId, unsigned int targetId);
	void checkProjectileLifetimes();
	void checkShipLifetime();
	void setShipDirection();
	void shipFireProjectile();

	void checkLevel();

	void addPointsFromActor(unsigned int id);
	void checkPlayerDeath();
};