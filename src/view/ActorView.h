#pragma once

#include "gl/VertexArray.h"
#include "gl/IndexBuffer.h"
#include "gl/Shader.h"
#include "gl/Texture.h"

#include <string>

class ActorView {
public:
	VertexArray va;
	VertexBuffer vb;
	IndexBuffer ib;
	Shader shader;
	Texture texture;
	unsigned int id;
	static float projectionMatrix[4][4];
	ActorView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionCount, std::string& shaderPath, std::string& texturePath, unsigned int id);
	~ActorView();

	ActorView(ActorView&& other) noexcept
		: va(other.va), vb(other.vb), ib(other.ib), shader(other.shader), texture(other.texture), id(other.id) {
		other.texture.m_RendererID = 0; //Use the "null" texture for the old object.
		other.shader.m_RendererID = 0; 
		other.va.m_RendererID = 0;
		other.vb.m_RendererID = 0;
		other.ib.m_RendererID = 0;
	}

	ActorView& operator=(ActorView&& other) noexcept {
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->texture.m_RendererID = 0; //Use the "null" texture for the old object.
			this->shader.m_RendererID = 0;
			this->va.m_RendererID = 0;
			this->vb.m_RendererID = 0;
			this->ib.m_RendererID = 0;
			
			std::swap(this->texture, other.texture);
			std::swap(this->shader, other.shader);
			std::swap(this->va, other.va);
			std::swap(this->vb, other.vb);
			std::swap(this->ib, other.ib);
		}
	}

	ActorView(const ActorView&) = delete;
	ActorView& operator=(const ActorView&) = delete;

	void SetPosition(float position[3], float angle);
};