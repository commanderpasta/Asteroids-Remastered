#include "ParticleModel.h"

ActorType getProjectileTypeFromSource(ActorType& sourceType) {
	switch (sourceType) {
	case ActorType::Player:
		return ActorType::PlayerParticle;
	default:
		return ActorType::Particle;
	}
}

ParticleModel::ParticleModel(steady_clock::time_point timeOfSpawn, float sourcePosition[3], ActorType sourceType) : timeOfSpawn(timeOfSpawn), ActorModel(sourcePosition, 0.0f, getProjectileTypeFromSource(sourceType)) {
}

ParticleModel::~ParticleModel() {}

bool ParticleModel::shouldDestroy(steady_clock::time_point currentFrameTime) {
	duration<double> timeSpan = duration_cast<duration<double>>(currentFrameTime - this->timeOfSpawn);
	return timeSpan.count() > 1;
}