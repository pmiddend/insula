#version 140

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 perspective;
in vec3 position;
out vec3 gradient_color;

const vec3 color0 = vec3(0.765,0.87,1.0);
const vec3 color1 = vec3(0.0,0.0,1.0);

void main()
{
	vec4 result = 
		perspective * 
		rotation * 
		translation * 
		vec4(position,1.0);
	gradient_color = position.y * color0 + (1.0 - position.y) * color1;
	gl_Position = result;
}
