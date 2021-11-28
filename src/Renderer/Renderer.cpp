#include "Renderer.hpp"
#include "../Macros.hpp"
#include "GL/glew.h"
#include "Basic.shader"
#include "../Variables.hpp"
#include "Uniform.hpp"
#include "Scene.hpp"

namespace Hexeng::Renderer
{

	Shader basic_shader;

	void init()
	{

		HXG_SGL(glEnable(GL_BLEND));
		HXG_SGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		HXG_SGL(glBlendEquation(GL_FUNC_ADD));

		basic_shader = { basic_vs, basic_fs };

		static Uniform<Vec2<float>> u_cam{&basic_shader, "u_cam", &cam_position};
		static Uniform<float> u_zoom{ &basic_shader, "u_zoom", &zoom };
		static Uniform<Vec2<float>> u_transform{&basic_shader, "u_transform", & transform};

	}

	void refresh_uniforms()
	{
		HXG_REFRESH_UNIFORM(float);
		HXG_REFRESH_UNIFORM(double);
		HXG_REFRESH_UNIFORM(int);

		HXG_REFRESH_UNIFORM(Vec2<float>);
		HXG_REFRESH_UNIFORM(Vec2<double>);
		HXG_REFRESH_UNIFORM(Vec2<int>);

		HXG_REFRESH_UNIFORM(Vec3<float>);
		HXG_REFRESH_UNIFORM(Vec3<double>);
		HXG_REFRESH_UNIFORM(Vec3<int>);

		HXG_REFRESH_UNIFORM(Vec4<float>);
		HXG_REFRESH_UNIFORM(Vec4<double>);
		HXG_REFRESH_UNIFORM(Vec4<int>);
	}

	void stop()
	{
		for (Layer*& layer : layers)
		{
			for (const auto& mesh : layer->meshes)
			{
				mesh->~Mesh();
				mesh->get_texture()->~Texture();
				mesh->access_shader()->~Shader();
			}
		}
	}

	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
	{
		va.bind();
		ib.bind();
		shader.bind();
		HXG_SGL(glDrawElements(GL_TRIANGLES, ib.get_count(), ib.get_type(), nullptr));
	}

	void clear()
	{
		HXG_SGL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void draw(const Mesh& mesh)
	{

		// todo : pre draw event

		mesh.get_texture()->bind();
		mesh.get_vao()->bind();
		mesh.get_shader()->bind();
		HXG_SGL(glDrawElements(GL_TRIANGLES, mesh.get_ib().get_count(), mesh.get_ib().get_type(), nullptr));

		// todo : post draw event

	}

	void draw(const Layer& layer)
	{
		for (const auto& mesh : layer.meshes)
		{
			draw(*mesh);
		}
	}

	void draw_scene(unsigned int scene_parameter)
	{
		for (Layer* lay : scenes[scene_parameter]->layers)
		{
			draw(*lay);
		}
	}

	void draw_current_scene()
	{
		for (Layer* lay : scenes[scene]->layers)
		{
			draw(*lay);
		}
		for (ContextualLayer* cl : contextual_layers)
		{
			if (*cl->context)
				draw(*cl);
		}
	}

}