#ifndef BASIC_SHADER
#define BASIC_SHADER

#define HXG_SHADER(x) #x

namespace Hexeng::Renderer
{

	const char basic_vs[] = HXG_SHADER
	(

		\n#version 460 core\n

		layout(location = 0) in vec4 position;
		layout(location = 1) in vec2 text_coord;
		out vec2 v_text_coord;

		uniform vec2 u_cam;
		uniform float u_zoom;
		uniform float u_scale;
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
			gl_Position = vec4((pos * sqrt(u_scale) - u_cam.xy + u_transform) * u_zoom, 0.0, 1.0);
			v_text_coord = text_coord;
		}

	);

	const char poly_vs[] = HXG_SHADER
	(
		\n#version 460 core\n

		layout(location = 0) in vec4 position;

		uniform vec2 u_cam;
		uniform float u_zoom;
		uniform float u_scale;
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
			gl_Position = vec4((pos * sqrt(u_scale) - u_cam.xy + u_transform) * u_zoom, 0.0, 1.0);
		}
	);

	const char poly_fs[] = HXG_SHADER
	(
		\n#version 460 core\n

		uniform vec4 u_color;

		out vec4 color;

		void main()
		{
			color = u_color;
		}
	
	);

	const char batching_vs[] = HXG_SHADER
	(
		\n#version 460 core\n

		layout(location = 0) in vec4 position;
		layout(location = 1) in vec2 text_coord;
		layout(location = 2) in float index;
		out vec2 v_text_coord;
		
		uniform vec2 u_cam;
		uniform float u_zoom;
		uniform float u_scale;
		uniform vec2 u_transform;
		uniform float u_rotation_angle;
		uniform ivec2 u_window_size;
		uniform vec4 u_color;
		
		uniform float u_quads_uniforms[1000];
		
		float to_radian(float degree)
		{
			return degree * 3.1415926538 / 180;
		}
		
		void main()
		{
		    vec2 quad_transform = vec2(u_quads_uniforms[int(index*4)], u_quads_uniforms[int(index*4+1)]);
			float angle = to_radian(u_rotation_angle + u_quads_uniforms[int(index*4)+3]);
			vec2 pos = vec2(position.x * cos(angle) + position.y * sin(angle) * u_window_size.y/u_window_size.x, position.y * cos(angle) - position.x * sin(angle) * u_window_size.x/u_window_size.y);
			gl_Position = vec4((pos * sqrt(u_scale * u_quads_uniforms[int(index*4)+2]) - u_cam.xy + u_transform + quad_transform) * u_zoom, 0.0, 1.0);
			v_text_coord = text_coord;
		}
	);

	const char font_fs[] = HXG_SHADER
	(

		\n#version 460 core\n

		in vec2 v_text_coord;
		out vec4 color;

		uniform sampler2D u_Texture;
		uniform vec4 u_color;

		void main()
		{
			color = vec4(u_color.xyz, texture(u_Texture, v_text_coord).r * u_color.w);
		}

	);

	const char line_fs[] = HXG_SHADER
	(

		\n#version 460 core\n

		out vec4 color;

		uniform vec4 u_color;

		void main()
		{
			color = u_color;
		}

	);

	const char basic_fs[] = HXG_SHADER
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

	const char tex_vector_vs[] = HXG_SHADER
	(

		\n#version 460 core\n

		layout(location = 0) in vec4 position;
		layout(location = 1) in vec2 text_coord;
		out vec2 v_text_coord;

		uniform sampler2D u_Texture;

		uniform vec2 u_cam;
		uniform float u_zoom;
		uniform float u_scale;
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
			gl_Position = vec4((pos * sqrt(u_scale) - u_cam.xy + u_transform) * u_zoom, 0.0, 1.0);
			vec2 text_size = textureSize(u_Texture, 0);
			v_text_coord = vec2(text_coord.x / text_size.x, text_coord.y / text_size.y);
		}

	);

}

#endif // !BASIC_SHADER