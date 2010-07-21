//#version 140

uniform mat4 mvp;
uniform sampler2D sand,rock,grass;
uniform vec2 grid_size;
attribute vec2 height_and_gradient;
varying vec2 texcoord;
varying vec2 height_and_gradient_out;
//varying vec3 position;

void main()
{
	//position = 
	//	vec3(gl_Vertex);

	const float multiplicator = 10.0;

	texcoord = 
		vec2(
			fract(gl_Vertex.x / grid_size.x * multiplicator),
			fract(gl_Vertex.z / grid_size.y * multiplicator));
	
	height_and_gradient_out = height_and_gradient;
	///texcoord = texture0_coord_in;

	//gl_Position = vec4(vec3(transpose(mvp) * gl_Vertex),1.0);
	gl_Position = transpose(mvp) * gl_Vertex;
	/*
	vec3 result = 
		vec3(
			transpose(mvp) *
			gl_Vertex);

	gl_Position = 
		vec4(
			result,
			1.0);
			*/
}
