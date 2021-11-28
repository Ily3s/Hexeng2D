#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include "GL/glew.h"
#include "../Macros.hpp"
#include "../Vectors.hpp"

namespace Hexeng::Renderer
{

	class Shader
	{

	private:

		unsigned int m_id;

		static unsigned int create_shader(const char* vertex_shader, const char* fragment_shader);
		static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);

		static unsigned int compile_shader(GLenum shader_type, const char* src);
		static unsigned int compile_shader(GLenum shader_type, const std::string& src);

	public:

		Shader() = default;

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&&) noexcept;
		Shader& operator=(Shader&&) noexcept;

		~Shader();

		Shader(const char* vertex_shader, const char* fragment_shader);
		Shader(const std::string& vertex_shader, const std::string& fragment_shader);

		void bind() const;
		void unbind() const;

		int get_uniform(const char* uniform) const;

		static void set_uniform(int uniform, const float value)				{ HXG_SGL(glUniform1f(uniform, value)); };
		static void set_uniform(int uniform, const Vec2<float>& value)		{ HXG_SGL(glUniform2f(uniform, value.x, value.y)); };
		static void set_uniform(int uniform, const Vec3<float>& value)		{ HXG_SGL(glUniform3f(uniform, value.x, value.y, value.z)); };
		static void set_uniform(int uniform, const Vec4<float>& value)		{ HXG_SGL(glUniform4f(uniform, value.x, value.y, value.z, value.w)); };

		static void set_uniform(int uniform, const double value)			{ HXG_SGL(glUniform1f(uniform, value)); };
		static void set_uniform(int uniform, const Vec2<double>& value)		{ HXG_SGL(glUniform2f(uniform, value.x, value.y)); };
		static void set_uniform(int uniform, const Vec3<double>& value)		{ HXG_SGL(glUniform3f(uniform, value.x, value.y, value.z)); };
		static void set_uniform(int uniform, const Vec4<double>& value)		{ HXG_SGL(glUniform4f(uniform, value.x, value.y, value.z, value.w)); };

		static void set_uniform(int uniform, const int value)				{ HXG_SGL(glUniform1f(uniform, value)); };
		static void set_uniform(int uniform, const Vec2<int>& value)		{ HXG_SGL(glUniform2f(uniform, value.x, value.y)); };
		static void set_uniform(int uniform, const Vec3<int>& value)		{ HXG_SGL(glUniform3f(uniform, value.x, value.y, value.z)); };
		static void set_uniform(int uniform, const Vec4<int>& value)		{ HXG_SGL(glUniform4f(uniform, value.x, value.y, value.z, value.w)); };

	};

}

#endif // !SHADER_HPP