#version 140

uniform sampler2D texture;
uniform bool reflection;
in vec4 texcoord;
out vec4 frag_color;

void main()
{
	if (reflection)
	{
		vec4 texvalue = 
			texture2D(
				texture,
				vec2(
					//(texcoord.x/texcoord.w),
					//(texcoord.y/texcoord.w)));
					(texcoord.x/texcoord.w) / 2.0 + 0.5,
					(texcoord.y/texcoord.w) / 2.0 + 0.5));

		frag_color = 
			vec4(
				vec3(texvalue),
				0.5);
	}
	else
	{
		frag_color = vec4(1,1,1,1);
	}
}
