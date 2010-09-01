#version 140

$$$HEADER$$$
in vec2 texcoord_out;
out vec4 frag_color;

void main()
{
	frag_color = 
		texture2D(
			texture,
			vec2(
				texcoord_out.s,
				texcoord_out.t));
}

