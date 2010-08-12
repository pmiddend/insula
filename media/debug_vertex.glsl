#version 140

uniform mat4 mvp;

in vec3 position;
in vec3 tip_color;
out vec3 color_out;

void main()
{
	gl_Position = 
		mvp * 
		vec4(position,1.0);

	color_out = tip_color;
}
