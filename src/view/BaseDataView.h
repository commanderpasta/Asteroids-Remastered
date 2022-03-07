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
	VertexArray va; /**< The <VertexArray> template for one actor type. */
	VertexBuffer vb; /**< The <VertexBuffer> template for one actor type. */
	IndexBuffer ib;  /**< The <IndexBuffer> template for one actor type. */
	
	BaseDataView(unsigned int indices[], float positions[], unsigned int vertexCount, unsigned int positionCount);
	~BaseDataView();

	/**
	 * A custom move constructor that works with OpenGL buffers.
	 * 
	 * It sets the renderer ids to 0 before dereferencing them as
	 * we don't want the buffers to actually be removed on the GPU side.
	 * 
	 * \param other The <BaseDataView> object to be moved.
	 * \return A new <BaseDataView> object with the same buffer references.
	 */
	BaseDataView(BaseDataView&& other) noexcept
		: va(other.va), vb(other.vb), ib(other.ib) {
		other.va.m_RendererID = 0;
		other.vb.m_RendererID = 0;
		other.ib.m_RendererID = 0;
	}

	/**
	 * A custom move constructor that works with OpenGL buffers.
	 *
	 * It sets the renderer ids to 0 before dereferencing them as
	 * we don't want the buffers to actually be removed on the GPU side.
	 *
	 * \param other The <BaseDataView> object to be moved.
	 * \return A new <BaseDataView> object with the same buffer references.
	 */
	BaseDataView& operator=(BaseDataView&& other) noexcept {
		// Check for self-assignment.
		if (this != &other)
		{
			this->va.m_RendererID = 0;
			this->vb.m_RendererID = 0;
			this->ib.m_RendererID = 0;

			std::swap(this->va, other.va);
			std::swap(this->vb, other.vb);
			std::swap(this->ib, other.ib);
		}
	}

	// Deny copying this object for consistency with OpenGL.
	BaseDataView(const BaseDataView&) = delete;
	BaseDataView& operator=(const BaseDataView&) = delete;
};