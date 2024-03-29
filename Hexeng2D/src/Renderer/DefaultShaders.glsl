#ifndef BASIC_SHADER
#define BASIC_SHADER

#include <string>

#define HXG_SHADER(x) #x

#ifndef Hexeng2D_EXPORTS
#endif

namespace Hexeng::Renderer
{

	std::string basic_vs = HXG_SHADER
	(

		\n#version 460 core\n

		layout(location = 0) in vec4 position;
		layout(location = 1) in vec2 text_coord;
		out vec2 v_text_coord;
		out vec2 vertex_position;

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
			gl_Position = vec4((pos * u_scale - u_cam.xy + u_transform) * u_zoom, 0.0, 1.0);
			v_text_coord = text_coord;
			vertex_position = gl_Position.xy;
		}

	);

	std::string poly_vs = HXG_SHADER
	(
		\n#version 460 core\n

		layout(location = 0) in vec4 position;

		out vec2 vertex_position;

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
			gl_Position = vec4((pos * u_scale - u_cam.xy + u_transform) * u_zoom, 0.0, 1.0);
			vertex_position = gl_Position.xy;
		}
	);

	std::string poly_fs = HXG_SHADER
	(
		\n#version 460 core\n

		uniform vec4 u_color_filter;
		uniform vec4 u_color;

		out vec4 color;

		void main()
		{
			color = u_color * u_color_filter;
		}
	
	);

	std::string batching_vs = HXG_SHADER
	(
		\n#version 460 core\n
		uniform float u_quads_uniforms[3000];

		layout(location = 0) in vec4 position;
		layout(location = 1) in vec2 text_coord;
		layout(location = 2) in float index;

		out vec2 v_text_coord;
		out vec2 vertex_position;
		out float fs_index;
		
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
		    vec2 quad_transform = vec2(u_quads_uniforms[int(index*12)], u_quads_uniforms[int(index*12+1)]);
			float angle = to_radian(u_rotation_angle + u_quads_uniforms[int(index*12)+3]);
			vec2 pos = vec2(position.x * cos(angle) + position.y * sin(angle) * u_window_size.y/u_window_size.x, position.y * cos(angle) - position.x * sin(angle) * u_window_size.x/u_window_size.y);
			gl_Position = vec4((pos * u_scale * u_quads_uniforms[int(index*12)+2] - u_cam.xy + u_transform + quad_transform) * u_zoom, 0.0, 1.0);
			v_text_coord = text_coord;
			vertex_position = gl_Position.xy;
			fs_index = index;
		}
	);

	std::string batching_fs = HXG_SHADER
	(

		\n#version 460 core\n
		uniform float u_quads_uniforms[3000];

		uniform vec4 u_color_filter;
		uniform vec4 u_color;

		in vec2 v_text_coord;
		out vec4 color;

		in float fs_index;

		uniform sampler2D u_Texture;

		void main()
		{
			vec4 quad_color_filter = vec4(u_quads_uniforms[int(fs_index*12+4)], u_quads_uniforms[int(fs_index*12+5)], u_quads_uniforms[int(fs_index*12+6)], u_quads_uniforms[int(fs_index*12+7)]);
			vec4 quad_color = vec4(u_quads_uniforms[int(fs_index*12+8)], u_quads_uniforms[int(fs_index*12+9)], u_quads_uniforms[int(fs_index*12+10)], u_quads_uniforms[int(fs_index*12+11)]);
			color = (texture(u_Texture, v_text_coord) + quad_color + u_color) * u_color_filter * quad_color_filter;
		}

	);

	std::string font_fs = HXG_SHADER
	(

		\n#version 460 core\n

		in vec2 v_text_coord;
		out vec4 color;

		uniform sampler2D u_Texture;
		uniform vec4 u_color_filter;
		uniform vec4 u_color;

		void main()
		{
			color = vec4(u_color.xyz * u_color_filter.xyz, (texture(u_Texture, v_text_coord).r + u_color.w) * u_color_filter.w);
		}

	);

	std::string line_fs = HXG_SHADER
	(

		\n#version 460 core\n

		out vec4 color;

		uniform vec4 u_color_filter;
		uniform vec4 u_color;

		void main()
		{
			color = u_color * u_color_filter;
		}

	);

	std::string basic_fs = HXG_SHADER
	(

		\n#version 460 core\n

		uniform vec4 u_color_filter;
		uniform vec4 u_color;

		in vec2 v_text_coord;
		out vec4 color;

		uniform sampler2D u_Texture;

		void main()
		{
			color = (texture(u_Texture, v_text_coord) + u_color) * u_color_filter;
		}

	);

	std::string tex_vector_vs = HXG_SHADER
	(

		\n#version 460 core\n

		layout(location = 0) in vec4 position;
		layout(location = 1) in vec2 text_coord;

		out vec2 v_text_coord;
		out vec2 vertex_position;

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
			gl_Position = vec4((pos * u_scale - u_cam.xy + u_transform) * u_zoom, 0.0, 1.0);
			vec2 text_size = textureSize(u_Texture, 0);
			v_text_coord = vec2(text_coord.x / text_size.x, text_coord.y / text_size.y);
			vertex_position = gl_Position.xy;
		}

	);

}

#endif // !BASIC_SHADER
