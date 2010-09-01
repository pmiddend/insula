#version 140

$$$HEADER$$$

void main()
{
	gl_PointSize = 20.0;
	gl_Position = mvp * vec4(position,1.0);
}
