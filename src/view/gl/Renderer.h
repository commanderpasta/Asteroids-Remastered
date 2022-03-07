#pragma once

#include <glew.h>

/**
 * Define wrappers around OpenGL functions for more verbose debugging messages.
 * 
 * Sets a breakpoint at the line where the error happened.
 */
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);