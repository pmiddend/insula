#version 140

$$$HEADER$$$
in vec2 texcoord_interp;
//in vec4 eye_space_interp;
in vec3 normal_interp;

out vec4 frag_color;

void main()
{
	vec4 color = 
		texture2D(
			texture,
			vec2(
				texcoord_interp.s,
				texcoord_interp.t));

	if (color.w < 0.5)
		discard;

	vec3 normalized_normal = 
		normalize(
			normal_interp);

	float diffuse = 
		0.4 
		+ clamp(
			dot(normalized_normal, normalize(light_source)), 0.0, 1.0);

	frag_color = 
		diffuse * color;
}

