#version 140

$$$HEADER$$$
/*
uniform mat4 mvp;
uniform vec3 sun_position;
uniform vec3 color0;
uniform vec3 color1;

in vec3 position;
*/

out vec3 gradient_color;
out float sun_intensity;
out vec3 position_out;

const float ypos = 0.5;

void main()
{
	vec4 result = 
		mvp * 
		vec4(
			position - vec3(0,ypos,0),
			1.0);

	gradient_color = 
		mix(
			color0,
			color1,
			position.y);

	position_out = vec3(position);

	gl_Position = result;
}
