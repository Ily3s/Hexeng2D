#ifndef MACROS_HPP
#define MACROS_HPP

#include <iostream>
#include <cassert>

/// @note HXG_DEBUG_LEVEL Should be defined before including this file.
/// Possible values : 0, 1, 2, 3
#ifndef HXG_DEBUG_LEVEL
#ifdef _DEBUG
#define HXG_DEBUG_LEVEL 2
#else
#define HXG_DEBUG_LEVEL 1
#endif
#endif

#if HXG_DEBUG_LEVEL >= 2
#define HXG_LOG_ERROR(str) std::cout << "[ERROR] " << str << std::endl; ::exit(-1)
#elif HXG_DEBUG_LEVEL == 1
#define HXG_LOG_ERROR(str) std::cout << "[ERROR] " << str << std::endl
#elif HXG_DEBUG_LEVEL == 0
#define HXG_LOG_ERROR(str)
#endif

#if HXG_DEBUG_LEVEL >= 3
#define HXG_LOG_WARNING(str) std::cout << "[WARNING] " << str << std::endl; ::exit(-1)
#elif HXG_DEBUG_LEVEL >= 1
#define HXG_LOG_WARNING(str) std::cout << "[WARNING] " << str << std::endl;
#elif HXG_DEBUG_LEVEL == 0
#define HXG_LOG_WARNING(str)
#endif

#if HXG_DEBUG_LEVEL != 0
#define HXG_ASSERT(cond, action) \
if (!cond) { action }""
#else
#define HXG_ASSERT(cond, action)
#endif

#if HXG_DEBUG_LEVEL >= 2
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

#if HXG_DEBUG_LEVEL >= 2
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