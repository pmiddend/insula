#version 140

out vec4 frag_color;
uniform vec3 sun_position;
uniform sampler2D sand,rock,grass;
in vec2 texcoord;
in vec2 height_and_gradient_out;
in float light_intensity;
in vec3 eye_space;

void main()
{
	float 
		grad = height_and_gradient_out.y,
		first = 1.0-height_and_gradient_out.x,
		second = height_and_gradient_out.x;

	vec4 light_color =
		light_intensity *
		(
			(1.0-grad) * first * 
			texture2D(
				sand,
				texcoord) +
			(1.0-grad) * second * 
			texture2D(
				grass,
				texcoord) +
			grad *
			texture2D(
				rock,
				texcoord));

	frag_color = light_color;
}

