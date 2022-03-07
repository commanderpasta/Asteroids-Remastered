#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h" 

/**
 * Parses the shader code and compiles the sources with OpenGL.
 * 
 * \param filepath The path of the file containing the shader code.
 */
Shader::Shader(const std::string& filepath) : m_FilePath(filepath), m_RendererID(0) {
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

/**
 * Deletes the corresponding shader in OpenGL.
 */
Shader::~Shader() {
    GLCall(glDeleteProgram(m_RendererID));
}

/**
 * Parses the shader and divides them into their vertex and fragment shader code.
 * 
 * The code should contain clear "#shader vertex" and "#shader fragment" lines to
 * recognize the segments in the parser.
 * 
 * \param filePath The path of the file containing the shader code.
 * \return The segmented parts of the shader code.
 */
ShaderProgramSource Shader::ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);
   
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) { //divide both segments by looking for string literals that define the sections
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

/**
 * Creates a shader program in OpenGL.
 * 
 * \param vertexShader The vertex shader code.
 * \param fragmentShader The fragment shader code.
 * \return The id of the newly created program in the API.
 */
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

/**
 * Compiles shader code.
 * 
 * \param type The OpenGL address for the shader type the source code is written for.
 * \param source Shader code written in GLSL.
 * \return 
 */
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*)malloc(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        free(message);

        GLCall(glDeleteShader(id));

        return 0;
    }

    return id;
}

/**
 * Binds the shader program in OpenGL.
 */
void Shader::Bind() const {
    GLCall(glUseProgram(m_RendererID));
}

/**
 * Unbinds the shader program in OpenGL.
 */
void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

/**
 * Modifies a uniform that takes an integer in the shader.
 * 
 * \param name The name of the uniform to be modified.
 * \param value The new value for the uniform.
 */
void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

/**
 * Modifies a uniform that takes a float in the shader.
 *
 * \param name The name of the uniform to be modified.
 * \param value The new value for the uniform.
 */
void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

/**
 * Modifies a uniform that takes a float pair in the shader.
 *
 * \param name The name of the uniform to be modified.
 * \param value The new value for the uniform.
 */
void Shader::SetUniform2f(const std::string& name, float v0, float v1) {
    GLCall(glUniform2f(GetUniformLocation(name), v0, v1));
}

/**
 * Modifies a uniform that takes four float values in the shader.
 *
 * \param name The name of the uniform to be modified.
 * \param value The new value for the uniform.
 */
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

/**
 * Modifies a 4x4 uniform matrix in the shader.
 *
 * \param name The name of the uniform to be modified.
 * \param value The new value for the uniform.
 */
void Shader::SetUniformMat4f(const std::string& name, float matrix[4][4]) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_TRUE, &matrix[0][0]));
}

/**
 * Gets the location of a uniform to access it through OpenGL.
 * 
 * Uses <m_UniformLocationCache> to look for cached values first, or caches it, if it
 * hasn't been cached before.
 * 
 * \param name The name of the uniform to search for.
 * \return The location id of the uniform.
 */
int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name]; //uniform location is already cached
    }

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}