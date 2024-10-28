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
		MULTIPLY,
		AVERAGE
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

		virtual void refresh(Shader* shad, const std::vector<void*>& values) = 0;
		virtual void refresh(const std::vector<void*>& values) = 0;

		void add_shaders(const std::vector<Shader*>& shad_list)
		{
			shader_list.reserve(shad_list.size());
			for (Shader* shad : shad_list)
				shader_list.insert({ shad, shad->get_uniform(uniform_name.c_str()) });
		}
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
		/// @pre shad must be a shader that implements this uniform and must be already bound.
		void refresh(Shader* shad) override;

		/// @brief Refreshes the uniform in the Shader shad, using value.
		/// @pre shad must be a shader that implements this uniform and must be already bound.
		void refresh(Shader* shad, void* value) override;

		/// @brief Refreshes the uniform in all shaders that implement it, using value.
		void refresh(void* value) override;

		/// @brief Refreshes the uniform (in the shader shad) using a multiples entries.
		/// @pre shad must be a shader that implements this uniform and must be already bound.
		void refresh(Shader* shad, const std::vector<void*>& values) override;

		/// @brief Refreshes the uniform using a multiples entries.
		void refresh(const std::vector<void*>& values) override;
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

		HXG_ASSERT((shader_list.find(shad) != shader_list.end()),
			HXG_LOG_ERROR("Shader not found"); return;);
		shad->set_uniform(shader_list[shad], *value_ptr);
	}

	template <typename VEC>
	void Uniform<VEC>::refresh(Shader* shad, void* value)
	{
		HXG_ASSERT((shader_list.find(shad) != shader_list.end()),
			HXG_LOG_ERROR("Shader not found"); return;);
		shad->set_uniform(shader_list[shad], *(reinterpret_cast<VEC*>(value)));
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
	void Uniform<VEC>::refresh(Shader* shad, const std::vector<void*>& values)
	{
		HXG_ASSERT((shader_list.find(shad) != shader_list.end()),
			HXG_LOG_ERROR("Shader not found"); return;);

		VEC result = 0;

		switch (fusion_mode)
		{
		case UniformFusionMode::SUM :
			for (void* val : values)
				result += *(reinterpret_cast<VEC*>(val));
			break;

		case UniformFusionMode::MULTIPLY :
			result += 1;
			for (void* val : values)
				result *= *(reinterpret_cast<VEC*>(val));
			break;

		case UniformFusionMode::AVERAGE :
			for (void* val : values)
				result += *(reinterpret_cast<VEC*>(val));
			result /= values.size();
			break;

		default :
			result = *(reinterpret_cast<VEC*>(values[0]));
			break;
		}

		shad->set_uniform(shader_list[shad], result);
	}

	template <typename VEC>
	void Uniform<VEC>::refresh(const std::vector<void*>& values)
	{
		for (auto& [shader, id] : shader_list)
		{
			shader->bind();
			refresh(shader, values);
		}
	}

}

#endif // !UNIFORM_HPP
