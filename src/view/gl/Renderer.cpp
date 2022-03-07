#include "Renderer.h"

#include <iostream>

/**
 * Checks if an OpenGL error occured.
 */
void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

/**
 * Logs an error from OpenGL in the debug console.
 * 
 * Outputs a formatted error message with a provided API error code, 
 * the affected function, file and line in the code.
 * 
 * 
 * \param function The function throwing the error.
 * \param file The file where the function was called.
 * \param line The line where the function was called.
 * \return 
 */
bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ": " << line << std::endl;
        return false;
    }
    return true;
}