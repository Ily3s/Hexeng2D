#include "Shader.hpp"

namespace Hexeng::Renderer
{

	unsigned int Shader::compile_shader(GLenum shader_type, const char* src)
	{
		HXG_SGL(unsigned int id = glCreateShader(shader_type));
		HXG_SGL(glShaderSource(id, 1, &src, nullptr));
		HXG_SGL(glCompileShader(id));

		int compile_status;
		HXG_SGL(glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status));
		if (compile_status == GL_FALSE)
		{
			int lenght;
			HXG_SGL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
			char* message = (char*)_malloca(lenght);
			HXG_SGL(glGetShaderInfoLog(id, lenght, &lenght, message));
			std::cout << "Failed to compile " << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
			std::cout << message << std::endl;
			HXG_SGL(glDeleteShader(id));
			return 0;
		}
		return id;
	}

	unsigned int Shader::compile_shader(GLenum shader_type, const std::string& source)
	{
		const char* src = source.c_str();
		return compile_shader(shader_type, src);
	}

	unsigned int Shader::create_shader(const char* vertex_shader, const char* fragment_shader)
	{
		HXG_SGL(unsigned int prog = glCreateProgram());
		unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
		unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

		HXG_SGL(glAttachShader(prog, vs));
		HXG_SGL(glAttachShader(prog, fs));
		HXG_SGL(glLinkProgram(prog));
		HXG_SGL(glValidateProgram(prog));

		HXG_SGL(glDeleteShader(vs));
		HXG_SGL(glDeleteShader(fs));

		return prog;
	}

	unsigned int Shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
	{
		const char* vs = vertex_shader.c_str();
		const char* fs = fragment_shader.c_str();
		return create_shader(vs, fs);
	}

	Shader::Shader(const char* vs, const char* fs)
		: m_id(create_shader(vs, fs)) {}

	Shader::Shader(const std::string& vs, const std::string& fs)
		: m_id(create_shader(vs, fs)) {}

	void Shader::bind() const
	{
		HXG_SGL(glUseProgram(m_id));
	}

	void Shader::unbind() const
	{
		HXG_SGL(glUseProgram(0));
	}

	int Shader::get_uniform(const char* uniform) const
	{
		HXG_SGL(int res = glGetUniformLocation(m_id, uniform));
		return res;
	}

	Shader::~Shader()
	{
		if (m_id)
		{
			HXG_SGL(glDeleteProgram(m_id));
			m_id = 0;
		}
	}

	Shader::Shader(Shader&& other) noexcept
		: m_id(other.m_id)
	{
		other.m_id = 0;
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		m_id = other.m_id;
		other.m_id = 0;
		return *this;
	}

}