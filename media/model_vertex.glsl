#version 140

$$$HEADER$$$
out vec2 texcoord_out;

void main()
{
	vec4 result = 
		mvp * 
		vec4(position,1.0);

	gl_Position = result;
	texcoord_out = texcoord;
}
