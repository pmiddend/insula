#version 140

uniform mat4 mvp;
uniform mat4 mvp_sun;
in vec3 position;
in vec2 mytexcoord;

out vec4 shadow_coord;
out vec2 texcoord_out;

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
	texcoord_out = mytexcoord;
}
