#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

/**
 * A C++ wrapper for vertex array objects in OpenGL.
 *
 * It wraps state information around an existing vertex buffer and is bound during render to provide all necessary vertex data.
 */
class VertexArray {
public: 
	unsigned int m_RendererID;

	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};