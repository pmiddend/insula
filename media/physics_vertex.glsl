#version 140

uniform mat4 mvp;

in vec3 position;

void main()
{
	vec4 result = 
		mvp * 
		vec4(position,1.0);

	gl_Position = result;
}
