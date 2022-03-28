#include "InitPresets.hpp"
#include "Basic.shader"
#include "BasicQuad.hpp"
#include "../Uniform.hpp"
#include "../Camera.hpp"
#include "../Renderer.hpp"

namespace Hexeng::Renderer::Presets
{

	Shader basic_shader;
	VertexLayout basic_vertex_layout;

	void init()
	{
		basic_shader = Shader(basic_vs, basic_fs);

		Camera::u_cam.add_shaders({ &basic_shader });
		Camera::u_zoom.add_shaders({ &basic_shader });
		u_transform.add_shaders({ &basic_shader });

		basic_vertex_layout = VertexLayout({ VertexElement{ 2, GL_FLOAT }, VertexElement{ 2, GL_FLOAT } });

		BasicQuad::init();
	}

	void stop()
	{
		BasicQuad::stop();
	}

}