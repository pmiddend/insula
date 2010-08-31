#version 140

uniform mat4 mvp;
uniform vec3 sun_position;
uniform vec3 color0;
uniform vec3 color1;

in vec3 position;
out vec3 gradient_color;
out float sun_intensity;
out vec3 position_out;

const float radius = 1.0;
const float ypos = 0.5;

void main()
{
	vec4 result = 
		mvp * 
		vec4(position - vec3(0,ypos,0),1.0);

	// The maximum distance of two points in the unit sphere is 2, so
	// divide by 2*radius here
	sun_intensity = 1.0 - length(position - sun_position)/(2*radius); 
	sun_intensity = sun_intensity * sun_intensity * sun_intensity;

	gradient_color = 
		mix(
			color0,
			color1,
			radius * position.y);

	position_out = vec3(position);

	gl_Position = result;
}
