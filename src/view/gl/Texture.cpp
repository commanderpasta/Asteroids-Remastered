#include "Texture.h"

#include <fstream>
#include <iostream>
#include <bitset>


/**
 * Creates a texture with OpenGL from a file.
 *
 * \param path The path of the texture file.
 */
Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0) {

	std::ifstream stream(path, std::ios::binary);

	int dataOffset;

	if (stream.is_open()) {
		// Use BITMAPFILEHEADER offsets to read metadata
		// Get total file size
		stream.seekg(0, std::ios::end);

		int fileSize = stream.tellg();

		if (path.find(".bff") != std::string::npos) {
			stream.seekg(2);
			stream.read((char*)(&m_Width), sizeof(int));
			stream.read((char*)(&m_Height), sizeof(int));

			dataOffset = 276;
		} else {

			// Get the offset for the image data
			stream.seekg(10);
			stream.read((char*)(&dataOffset), sizeof(int));

			// Get image height and width
			stream.seekg(18);
			stream.read((char*)(&m_Width), sizeof(int));
			stream.read((char*)(&m_Height), sizeof(int));
		}

		std::cout << "File size: " << fileSize << ", Data offset: " << dataOffset << ", widthOutput: " << m_Width << ", heightOutput: " << m_Height << std::endl;

		// Read image data
		stream.seekg(dataOffset);

		m_LocalBuffer = new unsigned char[fileSize];
		stream.read((char*)m_LocalBuffer, fileSize - dataOffset);

		stream.close();
	}

	// Create the texture in OpenGL
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	delete m_LocalBuffer;
}

/**
 * Deletes the corresponding texture in OpenGL.
 */
Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_RendererID));
}

/**
 * Binds the texture in OpenGL.
 * 
 * \param slot The texture slot to bind it to.
 */
void Texture::Bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

/**
 * Unbinds the texture in OpenGL.
 * 
 */
void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}