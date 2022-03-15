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
#include "ParticleModel.h"

#include "LevelSystem.h"
#include "physics/Engine.h"

#include "sound/DirectSound.h"

#include <vector>
#include <memory>
#include <map>
#include <chrono>

using namespace std::chrono;

class GameModel {
	std::shared_ptr<BaseShipModel> ship; /**< A reference to the current ship's <BaseShipModel> if there is one. */
	PhysicsEngine physicsEngine; /**< The physics engine that manages movement and collision for this model. */
	LevelSystem levelSystem; /**< The level system of this game instance. */

	steady_clock::time_point lastHyperSpaceActivation; /**< The last time the player has entered hyperspace. */
	bool playerIsInHyperSpace; /**< A bool that says whether the player is in hyperspace in the current tick. */
public:
	steady_clock::time_point lastPlayerDeath; /**< The last time the player has died. */

	std::shared_ptr<PlayerModel> player; /**< A reference to the player ship if it is alive. */

	unsigned int score; /**< The current game's score. */
	unsigned short playerLives; /**< The number of lives the player has remaining. */
	unsigned int pointsUntilExtraLive; /**< The amount of points the player has to get to get a bonus life. */

	unsigned int windowX; /**< The boundary of the game space's x-axis. */
	unsigned int windowY; /**< The boundary of the game space's y-axis. */
	std::vector<std::shared_ptr<AsteroidModel>> asteroids; /**< A list of all active large asteroids in the current tick. */
	std::vector<std::shared_ptr<MediumAsteroidModel>> mediumAsteroids; /**< A list of all active medium-sized asteroids in the current tick. */
	std::vector<std::shared_ptr<SmallAsteroidModel>> smallAsteroids; /**< A list of all active small asteroids in the current tick. */
	std::vector<std::shared_ptr<ProjectileModel>> projectiles; /**< A list of all active projectiles in the current tick. */
	std::vector <std::shared_ptr<ParticleModel>> particles; /**< A list of all active particles in the current tick. */
	std::map<unsigned int, std::shared_ptr<ActorModel>> actors; /**< A list of all active game objects in the current tick. */
	
	steady_clock::time_point currentTickTime; /**< The time point for the current tick. */

	/**
	 * An action that can be done for a sound.
	 */
	enum class SoundAction {
		PLAY, STOP, LOOP
	};

	/**
	 * An structure to describe a sound event.
	 * 
	 * Uses the sound's name and the action to perform to describe it.
	 * 
	 * @see <SoundAction>
	 */
	struct SoundEvent {
		SoundAction action;
		std::string soundName;
	};

	std::vector<std::string> soundFileList; /**< A list of all available sounds by their names. */
	std::vector<SoundEvent> soundEvents; /**< A list of all soundEvents to perform after this game tick. */

	bool backgroundSoundSwitch; /**< A switch to alternate between a high-/low pitch background sound. */
	steady_clock::time_point lastBackgroundSound; /**< The last time a background sound has been played. */

	GameModel(unsigned int windowX, unsigned int windowY);
	~GameModel();
	
	void setup();
	void setCurrentTime();

	void addPlayer(float startingPosition[2], float rotation);
	void addAsteroid(float startingPosition[2]);
	void addMediumAsteroid(float startingPosition[2]);
	void addSmallAsteroid(float startingPosition[2]);
	void addShip(bool isLarge);
	void spawnDeathParticles(float sourcePosition[2], ActorType sourceType);

	void removeActor(unsigned int id);

	void setPlayerAccelerating(bool isAccelerating);
	void playerRotateRight();
	void playerRotateLeft();
	void playerFireProjectile();

	void updatePositions();
	void checkCollisions();
	void checkCollisionWithProjectile(unsigned int projectileId, unsigned int targetId);
	void checkParticleLifetimes();
	void checkProjectileLifetimes();
	void checkShipLifetime();
	void setShipDirection();
	void shipFireProjectile();
	bool isGameOver();


	void checkLevel();

	void addPointsFromActor(unsigned int id);
	void checkPlayerDeath();
	void checkPlayerHyperSpace();
	void activateHyperSpace();

	void clearSoundEvents();
};