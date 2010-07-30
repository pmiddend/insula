#version 140

uniform mat4 perspective,rotation,translation;
uniform mat4 rotation_mirror,translation_mirror;
in vec3 position;
out vec4 texcoord;

void main()
{
	gl_Position = perspective * rotation * translation * vec4(position,1.0);
	texcoord = perspective * rotation_mirror * translation_mirror * vec4(position,1.0);
}
