#version 140

uniform mat4 mvp;
in vec3 position;

void main()
{
	gl_Position = transpose(mvp) * vec4(position,1.0);
}
