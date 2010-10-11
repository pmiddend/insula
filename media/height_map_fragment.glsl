#version 140

$$$HEADER$$$

in vec2 texcoord_interp;
in vec2 height_gradient_interp;
in vec4 shadow_coord_interp;
out vec4 frag_color;
	
float chebyshevUpperBound(vec4 coord)
{
	// We retrive the two moments previously stored (depth and
	// depth*depth)
	vec2 moments = texture2D(shadow_map,coord.xy).rg;

	// Surface is fully lit. as the current fragment is before the light
	// occluder
	if (coord.z <= moments.x)
		return 1.0 ;

	// The fragment is either in shadow or penumbra. We now use
	// chebyshev's upperBound to check How likely this pixel is to be
	// lit (p_max)
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance,0.00002);

	float d = coord.z - moments.x;
	float p_max = variance / (variance + d*d);

	return p_max;
}

void main()
{
	float 
		grad = height_gradient_interp.y,
		first = 1.0-height_gradient_interp.x,
		second = height_gradient_interp.x;

	vec4 real_shadow_coord = 
		shadow_coord_interp / shadow_coord_interp.w * 0.5 + 0.5;

	float shadow = chebyshevUpperBound(real_shadow_coord);
	//float shadow = texture2D(shadow_map,real_shadow_coord.xy).r;

	vec4 light_color =
		(1.0-grad) * first * 
		texture2D(
			sand,
			texcoord_interp) +
		(1.0-grad) * second * 
		texture2D(
			grass,
			texcoord_interp) +
		grad *
		texture2D(
			rock,
			texcoord_interp);

	frag_color = vec4(shadow * light_color.xyz,1.0);
}

