#ifndef UNIFORM_HPP
#define UNIFORM_HPP

#include "../Vectors.hpp"
#include "Shader.hpp"
#include <vector>
#include <unordered_map>
#include <string>

namespace Hexeng::Renderer
{

	template <typename VEC>
	class Uniform
	{
	private :

		std::string m_uniform_name;

	public :

		int id = 0;
		const VEC* value_ptr = nullptr;
		const Shader* shader = nullptr;

		std::unordered_map<Shader*, int> shader_list;

		static std::vector<Uniform*> s_uniform_list;

		Uniform(Shader* shad, const char* uniform_name, const VEC* value_ptr);

		Uniform() = default;

		Uniform(Uniform&&) noexcept;
		Uniform& operator=(Uniform&&) noexcept;

		void add_shaders(const std::vector<Shader*>& shad_list);

		void refresh();
		void refresh(Shader* shad);
	};

	template <typename VEC>
	std::vector<Uniform<VEC>*> Uniform<VEC>::s_uniform_list;

	template <typename VEC>
	Uniform<VEC>::Uniform(Shader* shad, const char* uniform_name, const VEC* value_ptr)
		: m_uniform_name(uniform_name), shader(shad), value_ptr(value_ptr)
	{
		id = shader->get_uniform(uniform_name);
		shader_list.insert({shad, id});
		Uniform<VEC>::s_uniform_list.push_back(this);
	}

	template <typename VEC>
	Uniform<VEC>::Uniform(Uniform&& moving) noexcept
		:	id (moving.id),
			value_ptr(moving.value_ptr),
			shader(moving.shader),
			shader_list(std::move(moving.shader_list)),
			m_uniform_name(std::move(moving.m_uniform_name))
	{
		auto it = std::find(s_uniform_list.begin(), s_uniform_list.end(), &moving);
		if (it != s_uniform_list.end())
			*it = this;
	}

	template <typename VEC>
	Uniform<VEC>& Uniform<VEC>::operator=(Uniform&& moving) noexcept
	{
		id = moving.id;
		value_ptr = moving.value_ptr;
		shader = moving.shader;
		shader_list = std::move(moving.shader_list);
		m_uniform_name = std::move(moving.m_uniform_name);
		auto it = std::find(s_uniform_list.begin(), s_uniform_list.end(), &moving);
		if (it != s_uniform_list.end())
			*it = this;
		return *this;
	}

	template <typename VEC>
	void Uniform<VEC>::add_shaders(const std::vector<Shader*>& shad_list)
	{
		shader_list.reserve(shad_list.size());
		for (Shader* shad : shad_list)
			shader_list.insert({shad, shad->get_uniform(m_uniform_name)});
	}

	template <typename VEC>
	void Uniform<VEC>::refresh()
	{
		shader->bind();
		shader->set_uniform(id, *value_ptr);
	}

	template <typename VEC>
	void Uniform<VEC>::refresh(Shader* shad)
	{
		auto it = shader_list.find(shad);
		shad->bind();
		shad->set_uniform(it->second, *value_ptr);
	}

}

#define HXG_REFRESH_UNIFORM(TYPE) \
for (auto& uniform : Uniform<TYPE>::s_uniform_list)\
{\
	uniform->shader->bind();\
	uniform->shader->set_uniform(uniform->id, *(uniform->value_ptr));\
}""

#endif // !UNIFORM_HPP