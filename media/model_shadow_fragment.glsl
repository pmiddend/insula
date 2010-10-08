#version 140

$$$HEADER$$$

in vec4 position_interp;
in vec2 texcoord_interp;
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

	float depth = position_interp.z / position_interp.w ;
	depth = depth * 0.5 + 0.5;
	
	float moment1 = depth;
	float moment2 = depth * depth;
	
	// Adjusting moments (this is sort of bias per pixel) using partial derivative
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25*(dx*dx+dy*dy);
	
	frag_color = vec4(moment1,moment2,0.0,0.0);
}

