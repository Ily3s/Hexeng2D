#include "Shader.hpp"
#include "../Macros.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "Uniform.hpp"

namespace Hexeng::Renderer
{

	unsigned int Shader::m_compile_shader(GLenum shader_type, const std::string& source)
	{
		const char* raw_src = source.c_str();
		HXG_GL(unsigned int id = glCreateShader(shader_type));
		HXG_GL(glShaderSource(id, 1, &raw_src, nullptr));
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

	static std::unordered_map<ShaderType, GLenum> shader_type_table
	{
		{ShaderType::VERTEX_SHADER, GL_VERTEX_SHADER},
		{ShaderType::TESSELLATION_CONTROL, GL_TESS_CONTROL_SHADER},
		{ShaderType::TESSELLATION_EVALUATION, GL_TESS_EVALUATION_SHADER},
		{ShaderType::GEOMETRY_SHADER, GL_GEOMETRY_SHADER},
		{ShaderType::FRAGMENT_SHADER, GL_FRAGMENT_SHADER},
		{ShaderType::COMPUTE_SHADER, GL_COMPUTE_SHADER}
	};

	unsigned int Shader::m_create_shader(const std::unordered_map<ShaderType, std::string>& source_code)
	{
		HXG_GL(unsigned int prog = glCreateProgram());
		
		std::vector<unsigned int> shaders_id;

		for (auto& [type, src] : source_code)
		{
			shaders_id.push_back(m_compile_shader(shader_type_table[type], src));
			HXG_GL(glAttachShader(prog, shaders_id.back()));
		}

		HXG_GL(glLinkProgram(prog));
		HXG_GL(glValidateProgram(prog));

		for (unsigned int shader_id : shaders_id)
		{
			HXG_GL(glDeleteShader(shader_id));
		}

		return prog;
	}

	Shader::Shader(const std::unordered_map<ShaderType, std::string>& source_code)
		: m_id(m_create_shader(source_code))
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
		: m_id(other.m_id), m_uniforms(std::move(other.m_uniforms))
	{
		other.m_id = 0;
		ToBeDelete(this, [this]() { this->~Shader(); });
		ToBeDelete::remove(&other);

		for (UniformInterface* ui : m_uniforms)
		{
			auto val = ui->shader_list.at(&other);
			ui->shader_list.erase(&other);
			ui->shader_list.insert({ this, val });
		}
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		m_id = other.m_id;
		other.m_id = 0;
		m_uniforms = std::move(other.m_uniforms);

		ToBeDelete(this, [this]() { this->~Shader(); });
		ToBeDelete::remove(&other);

		for (UniformInterface* ui : m_uniforms)
		{
			auto val = ui->shader_list.at(&other);
			ui->shader_list.erase(&other);
			ui->shader_list.insert({ this, val });
		}

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

		m_uniforms.insert(m_uniforms.end(), uniforms.begin(), uniforms.end());
	}

	void Shader::add_necessary_uniforms()
	{
		add_uniforms(UniformInterface::necessary_uniforms);
	}

}