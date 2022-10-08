#include "Shader.hpp"
#include "../Macros.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Uniform.hpp"

namespace Hexeng::Renderer
{

	unsigned int Shader::m_compile_shader(GLenum shader_type, const char* src)
	{
		HXG_GL(unsigned int id = glCreateShader(shader_type));
		HXG_GL(glShaderSource(id, 1, &src, nullptr));
		HXG_GL(glCompileShader(id));

		int compile_status;
		HXG_GL(glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status));
		HXG_ASSERT(compile_status,
			int lenght;
			HXG_GL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght));
			char* message = new char[lenght];
			HXG_GL(glGetShaderInfoLog(id, lenght, &lenght, message));
			std::cout << "Failed to compile " << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
			std::cout << message << std::endl;
			HXG_GL(glDeleteShader(id));
			delete[] message;
			return 0;
		);
		return id;
	}

	unsigned int Shader::m_compile_shader(GLenum shader_type, const std::string& source)
	{
		const char* src = source.c_str();
		return m_compile_shader(shader_type, src);
	}

	unsigned int Shader::m_create_shader(const char* vertex_shader, const char* fragment_shader)
	{
		HXG_GL(unsigned int prog = glCreateProgram());
		unsigned int vs = m_compile_shader(GL_VERTEX_SHADER, vertex_shader);
		unsigned int fs = m_compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

		HXG_GL(glAttachShader(prog, vs));
		HXG_GL(glAttachShader(prog, fs));
		HXG_GL(glLinkProgram(prog));
		HXG_GL(glValidateProgram(prog));

		HXG_GL(glDeleteShader(vs));
		HXG_GL(glDeleteShader(fs));

		return prog;
	}

	unsigned int Shader::m_create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
	{
		const char* vs = vertex_shader.c_str();
		const char* fs = fragment_shader.c_str();
		return m_create_shader(vs, fs);
	}

	Shader::Shader(const char* vs, const char* fs)
		: m_id(m_create_shader(vs, fs))
	{
		HXG_ASSERT((!missing_uniforms()),
			HXG_LOG_ERROR(std::to_string(missing_uniforms()) + " necessary uniforms are missing in a shader"););

		ToBeDelete(this, [this]() { this->~Shader(); });
	}

	Shader::Shader(const std::string& vs, const std::string& fs)
		: m_id(m_create_shader(vs, fs))
	{
		HXG_ASSERT((!missing_uniforms()),
			HXG_LOG_ERROR(std::to_string(missing_uniforms()) + " necessary uniforms are missing in a shader"););

		ToBeDelete(this, [this]() { this->~Shader(); });
	}

	void Shader::bind() const
	{
		HXG_GL(glUseProgram(m_id));
	}

	void Shader::unbind() const
	{
		HXG_GL(glUseProgram(0));
	}

	int Shader::get_uniform(const char* uniform) const
	{
		HXG_GL(int res = glGetUniformLocation(m_id, uniform));
		return res;
	}

	Shader::~Shader()
	{
		if (m_id)
		{
			HXG_GL(glDeleteProgram(m_id));
			m_id = 0;
		}
	}

	Shader::Shader(Shader&& other) noexcept
		: m_id(other.m_id)
	{
		other.m_id = 0;
		ToBeDelete(this, [this]() { this->~Shader(); });
		ToBeDelete::remove(&other);
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		m_id = other.m_id;
		other.m_id = 0;

		ToBeDelete(this, [this]() { this->~Shader(); });
		ToBeDelete::remove(&other);

		return *this;
	}

	std::vector<UniformInterface*> UniformInterface::necessary_uniforms;

	unsigned int Shader::missing_uniforms()
	{
		unsigned int output = 0;

		for (UniformInterface* ui : UniformInterface::necessary_uniforms)
		{
			if (get_uniform(ui->uniform_name.c_str()) == -1)
				output++;
		}

		return output;
	}

	void Shader::add_uniforms(const std::vector<UniformInterface*>& uniforms)
	{
		for (UniformInterface* ui : uniforms)
			ui->add_shaders({ this });
	}

	void Shader::add_necessary_uniforms()
	{
		add_uniforms(UniformInterface::necessary_uniforms);
	}

}