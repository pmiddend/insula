#version 140

$$$HEADER$$$

void main()
{
	gl_Position = 
		mvp 
			* vec4(position,1.0);
}
