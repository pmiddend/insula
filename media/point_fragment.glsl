#version 140

uniform sampler2D texture;

out vec4 frag_color;

void main()
{
	frag_color = 
		texture2D(
			texture,
			gl_PointCoord);
	//frag_color.w = 0.1;
}
