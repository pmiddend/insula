#version 140

$$$HEADER$$$

out vec4 position_interp;
out vec2 texcoord_interp;

void main()
{
	position_interp = mvp * vec4(position,1.0);
	texcoord_interp = texcoord;
	gl_Position = mvp * vec4(position,1.0);
	gl_PointSize = float(normal);
}
