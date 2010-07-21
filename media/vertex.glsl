//#version 140

uniform mat4 mvp;
uniform sampler2D main_texture;
uniform vec2 grid_size;
//attribute vec2 texture0_coord_in;
varying vec2 texcoord;
//varying vec3 position;

void main()
{
	//position = 
	//	vec3(gl_Vertex);

	texcoord = 
		vec2(
			gl_Vertex.x / grid_size.x,
			gl_Vertex.z / grid_size.y);
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
