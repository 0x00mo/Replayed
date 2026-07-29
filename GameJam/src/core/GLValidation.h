#pragma once
#include <GL/glew.h>

#include <iostream>

GLenum GlCheckError_(const char* file, int line);

#define GlCheckError() GlCheckError_(__FILE__, __LINE__) 

#ifdef _DEBUG
#define GlCall(func) func; \
		GlCheckError_(__FILE__, __LINE__)
#else
#define GlCall(func) func;
#endif