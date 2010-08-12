#version 140

in vec3 color_out;
out vec4 frag_color;

void main()
{
	frag_color = 
		vec4(color_out,1.0);
}

