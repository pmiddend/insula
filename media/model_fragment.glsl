#version 140

$$$HEADER$$$
in vec2 texcoord_interp;
in vec4 eye_space_interp;
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
	vec3 normalized_vertex_to_light = 
		normalize(
			light_source - vec3(eye_space_interp));

	float diffuse = clamp(dot(normal_interp, normalized_vertex_to_light), 0.0, 1.0);

	frag_color = 
		(0.4 + diffuse) * color;
}

