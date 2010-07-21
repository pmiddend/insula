//#version 140

uniform mat4 mvp;
uniform sampler2D sand,rock,grass;
uniform vec2 grid_size;
uniform vec3 sun_position;
uniform float ambient_light;
attribute vec3 normal;
attribute vec2 height_and_gradient;
varying vec2 texcoord;
varying vec2 height_and_gradient_out;
varying float light_intensity;

void main()
{
	const float multiplicator = 10.0;

	texcoord = 
		vec2(
			gl_Vertex.x / grid_size.x * multiplicator,
			gl_Vertex.z / grid_size.y * multiplicator);
	
	light_intensity = 
		max(
			ambient_light,
			max(
				0.0, 
				dot(normalize(normal), normalize(sun_position - gl_Vertex.xyz))));

	height_and_gradient_out = height_and_gradient;

	gl_Position = transpose(mvp) * gl_Vertex;
}
