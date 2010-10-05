#version 140

$$$HEADER$$$
out vec2 texcoord_interp;
//out vec4 eye_space_interp;
out vec3 normal_interp;

void main()
{
	gl_Position = 
		mvp 
			* vec4(position,1.0);
	texcoord_interp = texcoord;
	normal_interp = vec3(normal_matrix * vec4(normal,0.0));
	//eye_space_interp = mv * vec4(position,1.0);
}
