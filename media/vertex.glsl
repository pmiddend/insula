//#version 140

uniform mat4 world,perspective;
uniform sampler2D sand,rock,grass;
uniform vec2 grid_size;
uniform vec3 sun_direction;
uniform float ambient_light;
uniform float multiplicator;
attribute vec3 normal;
attribute vec2 height_and_gradient;
varying vec2 texcoord;
varying vec2 height_and_gradient_out;
varying float light_intensity;

varying vec3 eye_space;

void main()
{
	texcoord = 
		vec2(
			gl_Vertex.x / grid_size.x * multiplicator,
			gl_Vertex.z / grid_size.y * multiplicator);
	
	//normal = gl_NormalMatrix * gl_Normal;
	
	light_intensity = 
		max(
			ambient_light,
			max(
				0.0, 
				//dot(normalize(normal), normalize(sun_position - gl_Vertex.xyz))));
				dot(normalize(normal), normalize(sun_direction))));

	height_and_gradient_out = height_and_gradient;

	gl_Position = perspective * world * gl_Vertex;

	eye_space = vec3(world * gl_Vertex);
}
