#pragma once

#include "gl/VertexArray.h"
#include "gl/IndexBuffer.h"
#include "gl/Texture.h"

#include <string>

/**
 * Base class that wraps all vertex data for an object that can be drawn in the application.
 *
 * Can be reused across multiple object instances for better performance.
 */
class BaseDataView {
public:
	VertexArray va;
	VertexBuffer vb;
	IndexBuffer ib;
	BaseDataView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionCount);

	~BaseDataView();

	BaseDataView(BaseDataView&& other) noexcept
		: va(other.va), vb(other.vb), ib(other.ib) {
		//other.texture.m_RendererID = 0; //Use the "null" texture for the old object.
		other.va.m_RendererID = 0;
		other.vb.m_RendererID = 0;
		other.ib.m_RendererID = 0;
	}

	BaseDataView& operator=(BaseDataView&& other) noexcept {
		//ALWAYS check for self-assignment.
		if (this != &other)
		{
			//this->texture.m_RendererID = 0; //Use the "null" texture for the old object.
			this->va.m_RendererID = 0;
			this->vb.m_RendererID = 0;
			this->ib.m_RendererID = 0;

			//std::swap(this->texture, other.texture);
			std::swap(this->va, other.va);
			std::swap(this->vb, other.vb);
			std::swap(this->ib, other.ib);
		}
	}

	BaseDataView(const BaseDataView&) = delete;
	BaseDataView& operator=(const BaseDataView&) = delete;
};