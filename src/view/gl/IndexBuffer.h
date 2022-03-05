#pragma once

/**
 * A C++ wrapper for index buffers used in OpenGL.
 * 
 * An index buffer describes each vertex with an index, so they can be reused.
 */
class IndexBuffer {
	private:
		unsigned int m_Count; /**< count variable. Describes the amount of vertices. */
	public:
		unsigned int m_RendererID; /**< renderer ID. Unique identifier for the corresponding buffer in OpenGL. */

		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_Count; }
};