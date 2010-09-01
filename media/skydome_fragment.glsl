#version 140

$$$HEADER$$$
/*
uniform vec3 sun_position;
uniform vec3 color0;
uniform vec3 color1;
*/

out vec4 frag_color;
in vec3 gradient_color;
in float sun_intensity;
in vec3 position_out;

const vec4 sun_color = vec4(1.0,1.0,0.91,1.0);

void main()
{
	float sun_intensity = 1.0 - length(position_out - sun_position)/2; 
	sun_intensity = sun_intensity * sun_intensity * sun_intensity;
	
	frag_color = 
		mix(
			vec4(gradient_color,1.0),
			sun_color,
			sun_intensity);
}

