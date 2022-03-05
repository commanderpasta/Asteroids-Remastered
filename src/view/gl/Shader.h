#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

/**
 * A C++ wrapper for shader programs used in OpenGL.
 *
 * Loads a shader program from a shader source file, can be modified with uniform setters,
 * to allow for freely structured shader code.
 *
 * The shaders include color and position data.
 */
class Shader {
private:
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	unsigned int m_RendererID;
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, float matrix[4][4]);
private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	int GetUniformLocation(const std::string& name);
};