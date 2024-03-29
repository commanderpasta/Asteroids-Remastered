#pragma once

/**
 * A C++ wrapper for vertex buffers in OpenGL.
 *
 * Contains data for each vertex and an uv map for drawing on textures on them.
 */
class VertexBuffer {
	public:
		unsigned int m_RendererID; /**< The id for accessing the vertex buffer in OpenGL. */

		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
};