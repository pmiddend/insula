#version 140

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 perspective;
in vec3 position;

void main()
{
	vec4 result = 
		perspective * 
		rotation * 
		translation * 
		vec4(position,1.0);
	gl_Position = result;
}
