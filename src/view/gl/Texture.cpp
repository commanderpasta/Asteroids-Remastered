#include "Texture.h"

#include <fstream>
#include <iostream>
#include <bitset>

Texture::Texture(const std::string& path) 
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0) {

	// CUSTOM .BMP IMPLEMENTATION
	std::ifstream stream(path, std::ios::binary);

	int dataOffset;

	if (stream.is_open()) {
		// get total file size
		stream.seekg(0, std::ios::end);
		int fileSize = stream.tellg();

		// read offset of image data from file
		stream.seekg(10);
		stream.read((char*)(&dataOffset), sizeof(int));

		// read image height and width
		stream.seekg(18);
		stream.read((char*)(&m_Width), sizeof(int));
		stream.read((char*)(&m_Height), sizeof(int));
	
		std::cout << "File size: " << fileSize << ", Data offset: " << dataOffset << ", widthOutput: " << m_Width << ", heightOutput: " << m_Height << std::endl;
		
		// read image data
		stream.seekg(dataOffset);

		m_LocalBuffer = new unsigned char [fileSize];
		stream.read ((char*)m_LocalBuffer, fileSize - dataOffset);
		stream.close();
	}
	//END EXTRACT HERE

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}