#version 140

uniform float wave_height;
uniform float wind_speed;
uniform sampler2D reflection_texture;
uniform sampler2D bump_texture;
in vec4 texcoord_projected;
in vec2 texcoord_bump;
out vec4 frag_color;

void main()
{
	vec2 transposed_projected = 
		vec2(
			(texcoord_projected.x/texcoord_projected.w) / 2.0 + 0.5,
			(texcoord_projected.y/texcoord_projected.w) / 2.0 + 0.5);

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
			clamp(perturbed_coords,0.0,1.0));

	frag_color = real_color;
}
