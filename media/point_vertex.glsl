#version 140

uniform mat4 mvp;

in vec3 position;
in vec4 particle_color;

void main()
{
	gl_PointSize = 20.0;
	gl_Position = mvp * vec4(position,1.0);
}
