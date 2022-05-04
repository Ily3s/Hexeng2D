#ifndef UNIFORM_HPP
#define UNIFORM_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>

#include "../Vectors.hpp"
#include "Shader.hpp"
#include "Macros.hpp"

namespace Hexeng::Renderer
{

	class UniformInterface
	{
	public :

		UniformInterface() = default;

		std::unordered_map<Shader*, int> shader_list;
		std::string uniform_name;

		static HXG_DECLSPEC std::vector<UniformInterface*> necessary_uniforms;

		virtual void refresh(Shader* shad, void* value) = 0;
		virtual void refresh(Shader* shad) = 0;
		virtual void refresh() = 0;
		virtual void refresh(void* value) = 0;

		void add_shaders(const std::vector<Shader*>& shad_list)
		{
			shader_list.reserve(shad_list.size());
			for (Shader* shad : shad_list)
				shader_list.insert({ shad, shad->get_uniform(uniform_name.c_str()) });
		}

		virtual void sum_val(void* val1, void* val2) = 0;
		virtual void substract_val(void* val1, void* val2) = 0;
		virtual void mult_val(void* val1, void* val2) = 0;
		virtual void div_val(void* val1, void* val2) = 0;
		virtual void inverse_val(void* val) = 0;

	};

	template <typename VEC>
	class Uniform : public UniformInterface
	{
	public :

		const VEC* value_ptr = nullptr;

		Uniform(const char* uniform_name, const VEC* value_ptr = nullptr, const std::vector<Shader*>& shaders = {});

		Uniform() = default;

		Uniform(Uniform&&) noexcept;
		Uniform& operator=(Uniform&&) noexcept;

		void refresh();
		void refresh(Shader* shad) override;
		void refresh(Shader* shad, void* value) override;
		void refresh(void* value) override;

		void sum_val(void* val1, void* val2) override;
		void substract_val(void* val1, void* val2) override;
		void mult_val(void* val1, void* val2) override;
		void div_val(void* val1, void* val2) override;
		void inverse_val(void* val) override;
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
	void Uniform<VEC>::refresh()
	{
		if (!value_ptr)
			return;

		for (auto& [shader, id] : shader_list)
		{
			shader->bind();
			shader->set_uniform(id, *value_ptr);
		}
	}

	template <typename VEC>
	void Uniform<VEC>::refresh(Shader* shad)
	{
		if (!value_ptr)
			return;

		auto it = shader_list.find(shad);
		assert(it != shader_list.end());
		shad->bind();
		shad->set_uniform(it->second, *value_ptr);
	}

	template <typename VEC>
	void Uniform<VEC>::refresh(Shader* shad, void* value)
	{
		auto it = shader_list.find(shad);
		assert(it != shader_list.end());
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

	template <typename VEC>
	void Uniform<VEC>::sum_val(void* val1, void* val2)
	{
		*(VEC*)val1 += *(VEC*)val2;
	}

	template <typename VEC>
	void Uniform<VEC>::substract_val(void* val1, void* val2)
	{
		*(VEC*)val1 -= *(VEC*)val2;
	}

	template <typename VEC>
	void Uniform<VEC>::mult_val(void* val1, void* val2)
	{
		*(VEC*)val1 *= *(VEC*)val2;
	}

	template <typename VEC>
	void Uniform<VEC>::div_val(void* val1, void* val2)
	{
		*(VEC*)val1 /= *(VEC*)val2;
	}

	template <typename VEC>
	void Uniform<VEC>::inverse_val(void* val)
	{
		*(VEC*)val = -*(VEC*)val;
	}

}

#endif // !UNIFORM_HPP