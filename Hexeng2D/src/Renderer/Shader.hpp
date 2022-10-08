#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <vector>

#include "../Macros.hpp"
#include "glad/glad.h"
#include "../Macros.hpp"
#include "../Vectors.hpp"

namespace Hexeng::Renderer
{

	class UniformInterface;

	/// @note Most of things in this class are internal. You should just take a look at the constructor.
	class HXG_DECLSPEC Shader
	{
	public:

		Shader() = default;
		
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		
		Shader(Shader&&) noexcept;
		Shader& operator=(Shader&&) noexcept;
		
		~Shader();
		
		/// @param vertex_shader The source code (in glsl) of the vertex shader.
		/// @param fragment_shader The source code (in glsl) of the fragment shader.
		Shader(const char* vertex_shader, const char* fragment_shader);

		/// @param vertex_shader The source code (in glsl) of the vertex shader.
		/// @param fragment_shader The source code (in glsl) of the fragment shader.
		Shader(const std::string& vertex_shader, const std::string& fragment_shader);
		
		void bind() const;
		void unbind() const;

		void add_uniforms(const std::vector<UniformInterface*>& uniforms);
		void add_necessary_uniforms();
		
		int get_uniform(const char* uniform) const;
		 
		static void set_uniform(int uniform, const float value)				{ HXG_GL(glUniform1f(uniform, value)); };
		static void set_uniform(int uniform, const Vec2<float>& value)		{ HXG_GL(glUniform2f(uniform, value.x, value.y)); };
		static void set_uniform(int uniform, const Vec3<float>& value)		{ HXG_GL(glUniform3f(uniform, value.x, value.y, value.z)); };
		static void set_uniform(int uniform, const Vec4<float>& value)		{ HXG_GL(glUniform4f(uniform, value.x, value.y, value.z, value.w)); };
		
		static void set_uniform(int uniform, const double value)			{ HXG_GL(glUniform1d(uniform, value)); };
		static void set_uniform(int uniform, const Vec2<double>& value)		{ HXG_GL(glUniform2d(uniform, value.x, value.y)); };
		static void set_uniform(int uniform, const Vec3<double>& value)		{ HXG_GL(glUniform3d(uniform, value.x, value.y, value.z)); };
		static void set_uniform(int uniform, const Vec4<double>& value)		{ HXG_GL(glUniform4d(uniform, value.x, value.y, value.z, value.w)); };
		
		static void set_uniform(int uniform, const int value)				{ HXG_GL(glUniform1i(uniform, value)); };
		static void set_uniform(int uniform, const Vec2<int>& value)		{ HXG_GL(glUniform2i(uniform, value.x, value.y)); };
		static void set_uniform(int uniform, const Vec3<int>& value)		{ HXG_GL(glUniform3i(uniform, value.x, value.y, value.z)); };
		static void set_uniform(int uniform, const Vec4<int>& value)		{ HXG_GL(glUniform4i(uniform, value.x, value.y, value.z, value.w)); };

		unsigned int missing_uniforms();

	private:

		unsigned int m_id;

		static unsigned int m_create_shader(const char* vertex_shader, const char* fragment_shader);
		static unsigned int m_create_shader(const std::string& vertex_shader, const std::string& fragment_shader);

		static unsigned int m_compile_shader(GLenum shader_type, const char* src);
		static unsigned int m_compile_shader(GLenum shader_type, const std::string& src);
	};

}

#endif // !SHADER_HPP