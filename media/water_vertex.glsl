#version 140

$$$HEADER$$$

out vec4 texcoord_projected;
out vec2 texcoord_bump;

void main()
{
	gl_Position = 
		mvp * vec4(position,1.0);
	texcoord_projected = 
		mvp_mirror * vec4(position,1.0);
	texcoord_bump = 
		texture_coordinate + time * wind_speed * vec2(0,1);
}
