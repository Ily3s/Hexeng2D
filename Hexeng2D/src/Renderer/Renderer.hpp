#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Mesh.hpp"
#include "../Macros.hpp"
#include "../Vectors.hpp"
#include "Layer.hpp"
#include "Uniform.hpp"

namespace Hexeng::Renderer
{

	HXG_DECLSPEC void init();

	HXG_DECLSPEC void stop();

	HXG_DECLSPEC void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);

	HXG_DECLSPEC void clear();

	HXG_DECLSPEC void refresh_uniforms();

	HXG_DECLSPEC extern void draw_scene(int scene_parameter);

	HXG_DECLSPEC extern void draw_current_scene();

	HXG_DECLSPEC extern Uniform<Vec2<float>> u_transform;
	HXG_DECLSPEC extern Uniform<float> u_rotation_angle;
	HXG_DECLSPEC extern Uniform<Vec2<int>> u_window_size;

	HXG_DECLSPEC extern Vec2<float> transform;
	HXG_DECLSPEC extern float rotation_angle;

	HXG_DECLSPEC extern std::vector<UniformInterface*> uniform_list;

	class HXG_DECLSPEC ToBeInit
	{
	private:

		friend void init();
		static bool start_init;

	public:

		ToBeInit(std::function<void(void)> init_object, int order = 0);
	};

	class HXG_DECLSPEC ToBeDelete
	{
	private:

		friend void stop();
		static std::unordered_map<void*, std::function<void(void)>> objects;

	public:

		ToBeDelete(void* obj_ptr, std::function<void(void)> delete_function);

		static void remove(void* obj_ptr);
	};

	HXG_DECLSPEC extern std::vector<std::function<void(void)>> pending_actions;

}

#endif // !RENDERER_HPP