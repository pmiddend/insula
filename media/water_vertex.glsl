#version 140

uniform float wave_height;
uniform float wind_speed;
uniform mat4 perspective,rotation,translation;
uniform mat4 rotation_mirror,translation_mirror;
uniform float time;
in vec3 position;
in vec2 texture_coordinate;
out vec4 texcoord_projected;
out vec2 texcoord_bump;

void main()
{
	gl_Position = 
		perspective * rotation * translation * vec4(position,1.0);
	texcoord_projected = 
		perspective * rotation_mirror * translation_mirror * vec4(position,1.0);
	texcoord_bump = 
		texture_coordinate + time * wind_speed * vec2(0,1);
}
