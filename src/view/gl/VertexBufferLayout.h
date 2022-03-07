#pragma once

#include <vector>
#include <glew.h>
#include "Renderer.h"

/**
 * The structure of a single element of a vertex buffer to be used in the <VertexBufferLayout>.
 */
struct VertexBufferElement {
	unsigned int type; /**< The type of the values */
	unsigned int count; /**< The amount of values that describe the vertex */
	unsigned char isNormalized; /**< GL_FALSE */

	/**
	 * Gets the size of type supported by OpenGL for vertex buffer values.
	 * 
	 * \param type An OpenGL constant for the type
	 * \return Returns the size of a single type element
	 */
	static unsigned int GetSizeOfType(unsigned int type) {
		switch (type) {
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT: 
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
		}

		ASSERT(false);
		return 0;
	}
};

template <typename T>
constexpr bool always_false = false; //used for static_assert

/**
 * Describes a layout to be used when creating a vertex array. 
 * 
 * It provides correct strides for different data types and vertex counts, when setting vertex attribute pointers.
 */
class VertexBufferLayout {
private:
	std::vector<VertexBufferElement> m_Elements; /**< A list consisting of the elements of the layout. */
	unsigned int m_Stride; /**< The stride of this layout. */
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	/**
	 * Pushes new VertexBufferElements to the layout and changes the stride.
	 * 
	 * \param count The amount of values needed to describe the vertex.
	 */
	template<typename T>
	void Push(unsigned int count) {
		static_assert(always_false<T>, "must use correct specialization"); //Throw a compiler error when calling this function with the default template
	}

	template<>
	void Push<float>(unsigned int count) {
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count) {
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	/**
	 * Get a list of all elements for this layout.
	 * 
	 * \return A list containing the elements of the buffer layout.
	 */
	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

	/**
	 * Get the combined stride of all elements for the layout.
	 *
	 * \return The stride of this layout.
	 */
	inline unsigned int GetStride() const { return m_Stride;  }
};