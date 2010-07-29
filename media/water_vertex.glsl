#version 140

uniform mat4 perspective,rotation,translation;
in vec3 position;

void main()
{
	gl_Position = perspective * rotation * translation * vec4(position,1.0);
}
