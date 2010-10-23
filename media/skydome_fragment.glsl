#version 140

$$$HEADER$$$

in vec3 gradient_color_interp;
in vec3 position_interp;
out vec4 frag_color;

const vec4 sun_color = vec4(1.0,1.0,0.91,1.0);

void main()
{
	float sun_intensity = 1.0 - length(position_interp - sun_position)/2; 
	sun_intensity = sun_intensity * sun_intensity * sun_intensity;
	
	frag_color = 
		mix(
			vec4(gradient_color_interp,1.0),
			sun_color,
			sun_intensity);
}

