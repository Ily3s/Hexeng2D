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

	/// @brief Defines how should a uniform get "combined" with another uniform.
	/// @details For example, a position should get summed (SUM), but a zoom, should get multiplied (MULTIPLY)
	enum class UniformFusionMode
	{
		SUM,
		SUBSTRACT,
		MULTIPLY,
		DIVIDE
	};

	/// <summary>
	/// The virtual base class of all templated instances of Uniform
	/// </summary>
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

	/// <summary>
	/// An enum of settings you can specify the Uniform constructor by using UniformArgs.
	/// The documentation of the possible values of this enum concern what should be the value that goes with this enum in UniformArgs.
	/// </summary>
	/// @note The least important of these values are SHADERS, as you can just do uniform.add_shaders instead.
	enum class UniformArgType
	{
		/// @brief The name of the uniform, const char *
		NAME,
		/// @brief The "controller" of the uniform, const VEC *
		CONTROLLLER,
		/// @brief A list of all shaders that implement the uniform, const std::vector<Shader*>*
		SHADERS,
		// @brief The fusion mode of the uniform, const UniformFusionMode *
		FUSION_MODE
	};

	using UniformArg = std::pair<UniformArgType, const void*>;
	using UniformArgs = std::unordered_map<UniformArgType, const void*>;

	/// <typeparam name="VEC">The type of value that the uniform deals with.</typeparam>
	/// @note VEC is not necessarily a Vec<T>, it can also be a native type like int or float.
	template <typename VEC>
	class Uniform : public UniformInterface
	{
	public :

		const VEC* value_ptr = nullptr;

		Uniform(const UniformArgs& arguments = {});

		Uniform(Uniform&&) noexcept;
		Uniform& operator=(Uniform&&) noexcept;

		/// @brief Refreshes the uniform in all shaders that implement it, using the "controller" variable.
		void refresh();

		/// @brief Refreshes the uniform in the Shader shad, using the "controller" variable.
		void refresh(Shader* shad) override;

		/// @brief Refreshes the uniform in the Shader shad, using value.
		void refresh(Shader* shad, void* value) override;

		/// @brief Refreshes the uniform in all shaders that implement it, using value.
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