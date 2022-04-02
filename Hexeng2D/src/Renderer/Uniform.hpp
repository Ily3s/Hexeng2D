#ifndef UNIFORM_HPP
#define UNIFORM_HPP

#include "../Vectors.hpp"
#include "Shader.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include "Macros.hpp"

namespace Hexeng::Renderer
{

	class UniformInterface
	{
	public :

		UniformInterface() = default;

		std::unordered_map<Shader*, int> shader_list;
		std::string uniform_name;

		static std::vector<UniformInterface*> necessary_uniforms;

		virtual void refresh(Shader* shad, void* value) = 0;
		virtual void refresh(Shader* shad) = 0;
		virtual void refresh() = 0;
		virtual void refresh(void* value) = 0;
	};

	template <typename VEC>
	class Uniform : public UniformInterface
	{
	public :

		const VEC* value_ptr = nullptr;

		Uniform(const char* uniform_name, const VEC* value_ptr, const std::vector<Shader*>& shaders);

		Uniform() = default;

		Uniform(Uniform&&) noexcept;
		Uniform& operator=(Uniform&&) noexcept;

		void add_shaders(const std::vector<Shader*>& shad_list);

		void refresh();
		void refresh(Shader* shad) override;
		void refresh(Shader* shad, void* value) override;
		void refresh(void* value) override;
	};

	HXG_DECLSPEC extern std::vector<UniformInterface*> uniform_list;

	template <typename VEC>
	Uniform<VEC>::Uniform(const char* uniform_name_arg, const VEC* value_ptr, const std::vector<Shader*>& shaders)
		: value_ptr(value_ptr)
	{
		uniform_name = std::string(uniform_name_arg);
		for (Shader* shader : shaders)
		{
			int id = shader->get_uniform(uniform_name_arg);
			shader_list.insert({ shader, id });
		}
		uniform_list.push_back(this);
	}

	template <typename VEC>
	Uniform<VEC>::Uniform(Uniform&& moving) noexcept
		:	value_ptr(moving.value_ptr),
			shader_list(std::move(moving.shader_list)),
			uniform_name(std::move(moving.uniform_name))
	{
		auto it = std::find(uniform_list.begin(), uniform_list.end(), &moving);
		if (it != uniform_list.end())
			*it = this;
	}

	template <typename VEC>
	Uniform<VEC>& Uniform<VEC>::operator=(Uniform&& moving) noexcept
	{
		value_ptr = moving.value_ptr;
		shader_list = std::move(moving.shader_list);
		uniform_name = std::move(moving.uniform_name);
		auto it = std::find(uniform_list.begin(), uniform_list.end(), &moving);
		if (it != uniform_list.end())
			*it = this;
		return *this;
	}

	template <typename VEC>
	void Uniform<VEC>::add_shaders(const std::vector<Shader*>& shad_list)
	{
		shader_list.reserve(shad_list.size());
		for (Shader* shad : shad_list)
			shader_list.insert({shad, shad->get_uniform(uniform_name.c_str())});
	}

	template <typename VEC>
	void Uniform<VEC>::refresh()
	{
		for (auto& [shader, id] : shader_list)
		{
			shader->bind();
			shader->set_uniform(id, *value_ptr);
		}
	}

	template <typename VEC>
	void Uniform<VEC>::refresh(Shader* shad)
	{
		auto it = shader_list.find(shad);
		shad->bind();
		shad->set_uniform(it->second, *value_ptr);
	}

	template <typename VEC>
	void Uniform<VEC>::refresh(Shader* shad, void* value)
	{
		auto it = shader_list.find(shad);
		shad->bind();
		shad->set_uniform(it->second, *(reinterpret_cast<VEC*>(value)));
	}

	template <typename VEC>
	void Uniform<VEC>::refresh(void* value)
	{
		for (auto& [shader, id] : shader_list)
		{
			shader->bind();
			shader->set_uniform(id, *(reinterpret_cast<VEC*>(value)));
		}
	}

}

#endif // !UNIFORM_HPP