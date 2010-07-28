#version 140

out vec4 frag_color;
in vec3 gradient_color;
in float sun_intensity;

//const vec4 sun_color = vec4(1.0,0.784,0.129,1.0);
const vec4 sun_color = vec4(1.0,1.0,0.91,1.0);

void main()
{
	//frag_color = vec4(1.0,1.0,1.0,1.0);
	frag_color = 
		sun_intensity * sun_color + 
		(1-sun_intensity) * vec4(gradient_color,1.0);
	//gl_FragColor = vec4(1.0,1.0,1.0,1.0);
}

