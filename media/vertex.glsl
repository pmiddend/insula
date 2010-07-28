#version 140

uniform mat4 translation,rotation,perspective;
uniform sampler2D sand,rock,grass;
uniform vec2 grid_size;
uniform vec3 sun_direction;
uniform float ambient_light;
uniform float texture_scaling;
in vec3 position;
in vec3 normal;
in vec2 height_and_gradient;
out vec2 texcoord;
out vec2 height_and_gradient_out;
out float light_intensity;
out vec3 eye_space;

void main()
{
	texcoord = 
		vec2(
			position.x / grid_size.x * texture_scaling,
			position.z / grid_size.y * texture_scaling);
	
	//normal_out = gl_NormalMatrix * gl_Normal;
	
	light_intensity = 
		max(
			ambient_light,
			max(
				0.0, 
				//dot(normalize(normal), normalize(sun_position - gl_Vertex.xyz))));
				dot(normalize(normal), normalize(sun_direction))));

	height_and_gradient_out = height_and_gradient;

	eye_space = vec3(rotation * translation * vec4(position,1.0));
	gl_Position = perspective * rotation * translation * vec4(position,1.0);
}
