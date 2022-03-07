#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "Renderer.h"

/**
 * Creates an empty VAO in OpenGL.
 */
VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

/**
 * Deletes the corresponding VAO in OpenGL.
 */
VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

/**
 * Add an existing vertex buffer to the VAO.
 * 
 * \param vb The <VertexBuffer> to be bound.
 * \param layout The strides of the buffer vertices.
 */
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, //Create a vertex attribute pointer for each vertex values using the layout
			element.isNormalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

}

/**
 * Binds the VAO in OpenGL.
 */
void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

/**
 * Unbinds the VAO in OpenGL.
 */
void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}