#version 140

$$$HEADER$$$

out vec3 color_out;

void main()
{
	gl_Position = 
		mvp * 
		vec4(position,1.0);

	color_out = tip_color;
}
