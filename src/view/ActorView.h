#pragma once

#include "gl/Shader.h"
#include "ActorDataView.h"
#include <memory>

/**
 * Describes a single in-game actor so it can be rendered.
 *
 * One ActorView object describes one instance of a game object including its vertex data, texture and shader.
 */
class ActorView {
public: 
	bool isHidden;
	unsigned int id;
	Shader shader;
	std::shared_ptr<ActorDataView> data;

	ActorView(std::shared_ptr<ActorDataView> data, std::string shaderPath, unsigned int id, float x, float y);
	~ActorView();

	ActorView(ActorView&& other) noexcept
		: shader(std::move(other.shader)), data(std::move(other.data)), id(std::move(other.id)), isHidden(other.isHidden) {
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

	void SetPosition(float position[3], float angle);
	void setResolution(float x, float y);
	void setHidden(bool isHidden);
};