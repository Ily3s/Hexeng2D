#ifndef BASIC_SHADER
#define BASIC_SHADER

#define SHADER(x) #x

namespace Hexeng::Renderer::Presets
{

	const char basic_vs[] = SHADER
	(

		\n#version 460 core\n

		layout(location = 0) in vec4 position;
		layout(location = 1) in vec2 text_coord;
		out vec2 v_text_coord;

		uniform vec2 u_cam;
		uniform float u_zoom;
		uniform vec2 u_transform;
		uniform float u_rotation_angle;
		uniform ivec2 u_window_size;

		float to_radian(float degree)
		{
			return degree * 3.1415926538 / 180;
		}

		void main()
		{
			float angle = to_radian(u_rotation_angle);
			vec2 pos = vec2(position.x * cos(angle) + position.y * sin(angle) * u_window_size.y/u_window_size.x, position.y * cos(angle) - position.x * sin(angle) * u_window_size.x/u_window_size.y);
			gl_Position = vec4((pos - u_cam.xy + u_transform) * u_zoom, 0.0, 1.0);
			v_text_coord = text_coord;
		}

	);

	const char basic_fs[] = SHADER
	(

		\n#version 460 core\n

		in vec2 v_text_coord;
		out vec4 color;

		uniform sampler2D u_Texture;

		void main()
		{
			color = texture(u_Texture, v_text_coord);
		}

	);

	const char font_fs[] = SHADER
	(

		\n#version 460 core\n

		in vec2 v_text_coord;
		out vec4 color;

		uniform sampler2D u_Texture;
		uniform vec3 u_color;

		void main()
		{
			color = vec4(u_color, texture(u_Texture, v_text_coord).r);
		}

	);

	const char line_fs[] = SHADER
	(

		\n#version 460 core\n

		in vec2 v_text_coord;
		out vec4 color;

		uniform sampler2D u_Texture;
		uniform vec3 u_color;

		void main()
		{
			color = vec4(u_color, 1.0);
		}

	);

}

#endif // !BASIC_SHADER