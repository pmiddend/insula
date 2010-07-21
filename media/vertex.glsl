//#version 140

uniform mat4 mvp;
uniform sampler2D sand,rock,grass;
uniform vec2 grid_size;
uniform vec3 sun_position;
attribute vec2 height_and_gradient;
varying vec2 texcoord;
varying vec2 height_and_gradient_out;
varying float light_intensity;

const vec3 real_sun_pos = 
	vec3(
		-10,100,-10);

void main()
{
	const float multiplicator = 10.0;

	texcoord = 
		vec2(
			gl_Vertex.x / grid_size.x * multiplicator,
			gl_Vertex.z / grid_size.y * multiplicator);
			//fract(gl_Vertex.x / grid_size.x * multiplicator),
			//fract(gl_Vertex.z / grid_size.y * multiplicator));
	
	height_and_gradient_out = height_and_gradient;

	gl_Position = transpose(mvp) * gl_Vertex;
}
