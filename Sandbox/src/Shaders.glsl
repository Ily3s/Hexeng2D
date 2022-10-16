#ifndef SHADERS_GLSL
#define SHADERS_GLSL

#define SHADER(x) #x

const char custom_fs[] = SHADER(

	\n#version 460 core\n

	in vec2 v_text_coord;
	out vec4 color;
	
	uniform sampler2D u_Texture;
	uniform vec4 u_color;
	
	void main()
	{
		color = vec4(u_color.xyz, texture(u_Texture, v_text_coord).a);
	}
	
);

#endif