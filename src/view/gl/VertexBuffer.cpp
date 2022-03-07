#include "VertexBuffer.h"
#include "Renderer.h"

/**
 * Creates a vertex buffer in OpenGL.
 * 
 * \param data The values for the vertices.
 * \param size The size of <data>.
 */
VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

/**
 * Deletes the corresponding vertex buffer in OpenGL. 
 */
VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

/**
 * Binds the vertex buffer in OpenGL.
 */
void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

/**
 * Unbinds the vertex buffer in OpenGL.
 */
void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}