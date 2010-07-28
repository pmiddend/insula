#version 140

out vec4 frag_color;
in vec3 gradient_color;

void main()
{
	//frag_color = vec4(1.0,1.0,1.0,1.0);
	frag_color = vec4(gradient_color,1.0);
	//gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}

