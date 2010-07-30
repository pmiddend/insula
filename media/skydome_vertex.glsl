#version 140

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 perspective;
uniform vec3 sun_position;
uniform vec3 color0;
uniform vec3 color1;

in vec3 position;
out vec3 gradient_color;
out float sun_intensity;

const float radius = 1;
const float ypos = 0.5;

void main()
{
	vec4 result = 
		perspective * 
		rotation * 
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

	gl_Position = result;
}
