#include "ParticleModel.h"

/**
 * Determines the particle's <ActorType> depending of the source's <ActorType>.
 * 
 * \param sourceType The <ActorType> of the particle's source.
 * \return The particle's <ActorType> to use.
 */
ActorType getProjectileTypeFromSource(ActorType& sourceType) {
	switch (sourceType) {
	case ActorType::Player:
		return ActorType::PlayerParticle;
	default:
		return ActorType::Particle;
	}
}

/**
 * Creates a particle.
 * 
 * \param timeOfSpawn The time of when the particle has spawned.
 * \param sourcePosition The position of the particle's source.
 * \param sourceType The <ActorType> of the particle's source.
 */
ParticleModel::ParticleModel(steady_clock::time_point timeOfSpawn, float sourcePosition[3], ActorType sourceType) : timeOfSpawn(timeOfSpawn), ActorModel(sourcePosition, 0.0f, getProjectileTypeFromSource(sourceType)) {
}

ParticleModel::~ParticleModel() {}

/**
 * Decides whether the particle should be destoryed.
 * 
 * Particles have a lifetime of one second.
 * 
 * \param currentTickTime The time of the current game tick.
 * \return A bool that says whether the particle should be destroyed.
 */
bool ParticleModel::shouldDestroy(steady_clock::time_point currentTickTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentTickTime - this->timeOfSpawn);
	return timeSpan.count() > 1.0;
}