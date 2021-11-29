#ifndef UNIFORM_HPP
#define UNIFORM_HPP

#include "../Vectors.hpp"
#include "Shader.hpp"
#include <vector>

namespace Hexeng::Renderer
{

	template <typename VEC>
	class Uniform
	{
	public :

		int id;
		const VEC* value_ptr;
		const Shader* shader;

		static std::vector<Uniform*> s_uniform_list;

		Uniform(const Shader* shader, const char* uniform_name, const VEC* value_ptr);

		Uniform() = default;

		Uniform(Uniform&&) noexcept;
		Uniform& operator=(Uniform&&) noexcept;

		void refresh();
	};

	template <typename VEC>
	std::vector<Uniform<VEC>*> Uniform<VEC>::s_uniform_list;

	template <typename VEC>
	Uniform<VEC>::Uniform(const Shader* shader, const char* uniform_name, const VEC* value_ptr)
	{
		int uniform_id = shader->get_uniform(uniform_name);
		Uniform<VEC>::s_uniform_list.emplace_back(uniform_id, value_ptr, shader);
	}

	template <typename VEC>
	Uniform<VEC>::Uniform(Uniform&& moving) noexcept
		:	id (moving.id),
			value_ptr(moving.value_ptr),
			shader(moving.shader)
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
		auto it = std::find(s_uniform_list.begin(), s_uniform_list.end(), &moving);
		if (it != s_uniform_list.end())
			*it = this;
		return *this;
	}

	template <typename VEC>
	void Uniform<VEC>::refresh()
	{
		shader->bind();
		shader->set_uniform(id, *value_ptr);
	}

}

#define HXG_REFRESH_UNIFORM(TYPE) \
for (auto& uniform : Uniform<TYPE>::s_uniform_list)\
{\
	uniform->shader->bind();\
	uniform->shader->set_uniform(uniform->id, *uniform->value_ptr);\
}""

#endif // !UNIFORM_HPP