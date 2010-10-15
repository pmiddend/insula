#version 140

$$$HEADER$$$
out vec4 frag_color;

void main()
{
	frag_color = 
		texture2D(
			texture,
			gl_FragCoord.xy / screen_size);
}

