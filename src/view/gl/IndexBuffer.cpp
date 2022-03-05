#include "IndexBuffer.h"
#include "Renderer.h"

/**
 * Generates an index buffer through OpenGL.
 * 
 * \param data An array describing the indices of each vertex
 * \param count Describes the amount of vertices
 */
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

/**
 * Deletes the corresponding index buffer in OpenGL.
 *
 */
IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

/**
 * Binds the index buffer in OpenGL.
 */
void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

/**
 * Unbinds the index buffer in OpenGL.
 */
void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}