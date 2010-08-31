#version 140

$$$HEADER$$$

in vec2 texcoord;
in vec2 height_and_gradient_out;
in float light_intensity;
out vec4 frag_color;

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

