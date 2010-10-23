#version 140

$$$HEADER$$$
out vec4 frag_color;
in vec4 screen_space;

void main()
{
	frag_color = 
		texture2D(
			texture,
			(screen_space / screen_space.w * 0.5 + 0.5).xy);
			//gl_FragCoord.xy / screen_size);
	//		gl_FragCoord.xy / gl_FragCoord.w);
}

