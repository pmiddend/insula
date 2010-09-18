#version 140

$$$HEADER$$$

out vec4 shadow_coord;

void main()
{
	shadow_coord = 
		/*mat4(
			0.5,0.0,0.0,0.0,
			0.0,0.5,0.0,0.0,
			0.0,0.0,0.5,0.0,
			0.5,0.5,0.5,1.0) * */
		mvp_sun * 
		vec4(position,1.0);
	gl_Position = mvp * vec4(position,1.0);
}
