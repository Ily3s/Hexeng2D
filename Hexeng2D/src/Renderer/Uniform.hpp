#ifndef UNIFORM_HPP
#define UNIFORM_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <cassert>
#include <algorithm>

#include "../Vectors.hpp"
#include "Shader.hpp"
#include "Macros.hpp"

namespace Hexeng::Renderer
{

	enum class UniformFusionMode
	{
		SUM,
		SUBSTRACT,
		MULTIPLY,
		DIVIDE
	};

	class UniformInterface
	{
	public :

		UniformInterface() = default;

		std::unordered_map<Shader*, int> shader_list;
		std::string uniform_name;

		UniformFusionMode fusion_mode = UniformFusionMode::SUM;

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

		void fusion_val(void* val1, void* val2)
		{
			switch (fusion_mode)
			{
			case UniformFusionMode::SUM :
				sum_val(val1, val2);
				break;

			case UniformFusionMode::SUBSTRACT :
				substract_val(val1, val2);
				break;

			case UniformFusionMode::MULTIPLY :
				mult_val(val1, val2);
				break;

			case UniformFusionMode::DIVIDE :
				div_val(val1, val2);
				break;

			default :
				break;
			}
		}

		void unfusion_val(void* val1, void* val2)
		{
			switch (fusion_mode)
			{
			case UniformFusionMode::SUM:
				substract_val(val1, val2);
				break;

			case UniformFusionMode::SUBSTRACT:
				sum_val(val1, val2);
				break;

			case UniformFusionMode::MULTIPLY:
				div_val(val1, val2);
				break;

			case UniformFusionMode::DIVIDE:
				mult_val(val1, val2);
				break;

			default:
				break;
			}
		}

		virtual void sum_val(void* val1, void* val2) = 0;
		virtual void substract_val(void* val1, void* val2) = 0;
		virtual void mult_val(void* val1, void* val2) = 0;
		virtual void div_val(void* val1, void* val2) = 0;
		virtual void inverse_val(void* val) = 0;

	};

	enum class UniformArgType
	{
		// const char *
		NAME,
		// const VEC *
		CONTROLLLER,
		// const std::vector<Shader*>*
		SHADERS,
		// const UniformFusionMode *
		FUSION_MODE
	};

	using UniformArg = std::pair<UniformArgType, const void*>;
	using UniformArgs = std::unordered_map<UniformArgType, const void*>;

	template <typename VEC>
	class Uniform : public UniformInterface
	{
	public :

		const VEC* value_ptr = nullptr;

		Uniform(const UniformArgs& arguments = {});

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
	Uniform<VEC>::Uniform(const UniformArgs& arguments)
	{
		for (const auto& [arg_t, val] : arguments)
		{
			switch (arg_t)
			{
			case UniformArgType::NAME :
				uniform_name = std::string(reinterpret_cast<const char*>(val));
				break;
			
			case UniformArgType::CONTROLLLER :
				value_ptr = reinterpret_cast<const VEC*>(val);
				break;

			case UniformArgType::SHADERS :
				for (Shader* shader : *reinterpret_cast<const std::vector<Shader*>*>(val))
				{
					int id = shader->get_uniform(uniform_name.c_str());
					shader_list.insert({ shader, id });
				}
				break;

			case UniformArgType::FUSION_MODE :
				fusion_mode = *reinterpret_cast<const UniformFusionMode*>(&val);
				break;

			default :
				break;
			}
		}
		uniform_list.push_back(this);
	}

	template <typename VEC>
	Uniform<VEC>::Uniform(Uniform&& moving) noexcept
		:	value_ptr(moving.value_ptr)
	{
        shader_list = std::move(moving.shader_list);
        uniform_name = std::move(moving.uniform_name);
        fusion_mode = moving.fusion_mode;
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
		fusion_mode = moving.fusion_mode;
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