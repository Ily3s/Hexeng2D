#ifndef BASIC_SHADER
#define BASIC_SHADER

#define SHADER(x) #x

namespace Hexeng::Renderer::Presets
{

	const char basic_vs[] = SHADER(

		\n#version 330 core\n

		layout(location = 0) in vec4 position;
		layout(location = 1) in vec2 text_coord;
		out vec2 v_text_coord;

		uniform vec2 u_cam;
		uniform float u_zoom;
		uniform vec2 u_transform;

		void main()
		{
			gl_Position = vec4((position.xy - u_cam.xy + u_transform) * u_zoom, 0.0, 1.0);
			v_text_coord = text_coord;
		}

	);

	const char basic_fs[] = SHADER(

		\n#version 330 core\n

		in vec2 v_text_coord;
		out vec4 color;

		uniform sampler2D u_Texture;

		void main()
		{
			color = texture(u_Texture, v_text_coord);
		}

	);

}

#endif // !BASIC_SHADER