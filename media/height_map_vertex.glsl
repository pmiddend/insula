#version 140

uniform mat4 translation,rotation,perspective;
uniform sampler2D sand,rock,grass;
uniform vec2 grid_size;
uniform vec3 sun_direction;
uniform float ambient_light;
uniform float texture_scaling;
uniform bool do_clip;
uniform float water_level;
in vec3 position;
in vec3 normal;
in vec2 height_and_gradient;
out vec2 texcoord;
out vec2 height_and_gradient_out;
out float light_intensity;

void main()
{
	texcoord = 
		texture_scaling * position.xz / grid_size;
	
	light_intensity = 
		max(
			ambient_light,
			max(
				0.0, 
				dot(normal, sun_direction)));

	height_and_gradient_out = height_and_gradient;

	gl_Position = perspective * rotation * translation * vec4(position,1.0);

	if (do_clip)
		gl_ClipDistance[0] = position.y - water_level;
}
