#version 140

$$$HEADER$$$

out vec3 gradient_color_interp;
out vec3 position_interp;

const float ypos = 0.5;

void main()
{
	vec4 result = 
		mvp * 
		vec4(
			position - vec3(0,ypos,0),
			1.0);

	gradient_color_interp = 
		mix(
			color0,
			color1,
			position.y);

	position_interp = vec3(position);
	gl_Position = result;
}
