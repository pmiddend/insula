#version 140

$$$HEADER$$$
out vec4 screen_space;

void main()
{
	gl_PointSize = float(screen_size);
	screen_space = mvp 
			* vec4(position,1.0);
	gl_Position = 
		mvp 
			* vec4(position,1.0);
}
