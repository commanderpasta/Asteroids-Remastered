#pragma once

class VertexBuffer {
	public:
		unsigned int m_RendererID;

		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
};