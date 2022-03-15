#pragma once

#include <memory>
#include <chrono>

#include "gl/Shader.h"
#include "ActorDataView.h"

using namespace std::chrono;

/**
 * Describes a single in-game actor so it can be rendered.
 *
 * One ActorView object describes one instance of a game object including its vertex data, texture and shader.
 */
class ActorView {
public:
	unsigned int id;
	Shader shader;
	std::shared_ptr<ActorDataView> data;

	bool isVisible;
	float flickerFrequency;
	steady_clock::time_point lastFlicker;

	ActorView(std::shared_ptr<ActorDataView> data, std::string shaderPath, unsigned int id, float windowX, float windowY);
	~ActorView();

	ActorView(ActorView&& other) noexcept
		: shader(std::move(other.shader)), data(std::move(other.data)), id(std::move(other.id)), flickerFrequency(std::move(other.flickerFrequency)) {
		other.shader.m_RendererID = 0;
	}

	ActorView& operator=(ActorView&& other) noexcept {
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->shader.m_RendererID = 0;
			std::swap(this->shader, other.shader);
		}
	}

	void setPosition(float position[2], float angle);
	void setResolution(float x, float y);
	void setFlickerFrequency(float frequency);
	void updateFlickering(steady_clock::time_point currentTickTime);
};