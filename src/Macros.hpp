#ifndef MACROS_HPP
#define MACROS_HPP

#include <iostream>

#ifdef _DEBUG
#define HXG_SGL(x) \
\
while (glGetError() != GL_NO_ERROR);\
x;\
{\
	int i = 0;\
	for (; GLenum error = glGetError(); ++i)\
	{\
		std::cout << "[OpenGL Error] (" << error << ") in file \"" << __FILE__ << "\" at line " << __LINE__\
		<< " at function \"" << #x << '\"' << std::endl;\
	}\
	if (i)\
		__debugbreak();\
}""
#else
#define HXG_SGL(x) x
#endif

#endif // !MACROS_HPP