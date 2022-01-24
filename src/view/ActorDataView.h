#pragma once

#include "BaseDataView.h"

#include <string>

class ActorDataView : public BaseDataView {
public:
	Texture texture;
	ActorDataView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionCount, std::string& texturePath);
	~ActorDataView();

	ActorDataView(ActorDataView&& other) noexcept
		: BaseDataView(std::move(other)), texture(std::move(other.texture)) {
		other.texture.m_RendererID = 0; //Use the "null" texture for the old object.
		other.va.m_RendererID = 0;
		other.vb.m_RendererID = 0;
		other.ib.m_RendererID = 0;
	}

	ActorDataView& operator=(ActorDataView&& other) noexcept {
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			this->texture.m_RendererID = 0; //Use the "null" texture for the old object.
			this->va.m_RendererID = 0;
			this->vb.m_RendererID = 0;
			this->ib.m_RendererID = 0;
			
			std::swap(this->texture, other.texture);
			std::swap(this->va, other.va);
			std::swap(this->vb, other.vb);
			std::swap(this->ib, other.ib);
		}
	}

	ActorDataView(const ActorDataView&) = delete;
	ActorDataView& operator=(const ActorDataView&) = delete;
};