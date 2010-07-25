#version 140

out vec4 frag_color;
uniform vec3 sun_position;
uniform sampler2D sand,rock,grass;
in vec2 texcoord_out;
in vec2 height_and_gradient;
in float light_intensity;
in vec3 eye_space;

void main()
{
	float 
		grad = height_and_gradient.y,
		first = 1.0-height_and_gradient.x,
		second = height_and_gradient.x;

	vec4 light_color =
		light_intensity *
		(
			(1.0-grad) * first * 
			texture2D(
				sand,
				texcoord_out) +
			(1.0-grad) * second * 
			texture2D(
				grass,
				texcoord_out) +
			grad *
			texture2D(
				rock,
				texcoord_out));

	frag_color = light_color;
}

