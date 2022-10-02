#ifndef MACROS_HPP
#define MACROS_HPP

#include <iostream>

#ifdef _DEBUG
#define HXG_GL(x) \
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
#define HXG_GL(x) x
#endif

#ifdef _DEBUG
#define HXG_GLFW(x) \
\
while (glfwGetError(NULL) != GLFW_NO_ERROR);\
x;\
{\
	int i = 0;\
	for (; int error = glfwGetError(NULL); ++i)\
	{\
		std::cout << "[GLFW Error] (" << error << ") in file \"" << __FILE__ << "\" at line " << __LINE__\
		<< " at function \"" << #x << '\"' << std::endl;\
	}\
	if (i)\
		__debugbreak();\
}""
#else
#define HXG_GLFW(x) x
#endif

#ifdef _MSC_VER
    #ifdef Hexeng2D_EXPORTS
        #define HXG_DECLSPEC __declspec(dllexport)
    #else
        #define HXG_DECLSPEC __declspec(dllimport)
    #endif
#elif defined(__GNUC__)
    #ifdef Hexeng2D_EXPORTS
        #define HXG_DECLSPEC __attribute__((visibility("default")))
    #else
        #define HXG_DECLSPEC
    #endif
#else
    #define  HXG_DECLSPEC
#endif

#endif // !MACROS_HPP