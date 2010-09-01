#version 140

$$$HEADER$$$

in vec4 texcoord_projected;
in vec2 texcoord_bump;
out vec4 frag_color;

void main()
{
	vec2 transposed_projected = 
		texcoord_projected.xy / texcoord_projected.w * 0.5 + 0.5;

	transposed_projected = 
		clamp(
			transposed_projected,
			0.001,
			0.999);

	vec4 bump_color = 
		texture2D(
			bump_texture,
			texcoord_bump);

	vec2 perturbation = 
		wave_height * (bump_color.rg - 0.5);

	vec2 perturbed_coords = 
		transposed_projected + perturbation;

	vec4 real_color = 
		texture2D(
			reflection_texture,
			clamp(perturbed_coords,0.001,0.999));

	frag_color = real_color;
}
