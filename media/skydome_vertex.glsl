#version 140

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 world;
uniform mat4 perspective;
uniform vec3 sun_position;
in vec3 position;
out vec3 gradient_color;
out float sun_intensity;

const vec3 color0 = vec3(0.765,0.87,1.0);
const vec3 color1 = vec3(0.0,0.0,1.0);

const float radius = 1;
const float ypos = 0.5;

void main()
{
	vec4 result = 
		perspective * 
//		world *
		rotation * 
	//	translation * 
		vec4(position - vec3(0,ypos,0),1.0);

	// The maximum distance of two points in the unit sphere is 2, so
	// divide by 2*radius here
	sun_intensity = 1 - length(position - sun_position)/(2*radius); 
	sun_intensity = sun_intensity * sun_intensity * sun_intensity;

	gradient_color = 
		radius * position.y * color1 + 
		(1.0 - radius * position.y) * color0;
	gl_Position = result;
}
