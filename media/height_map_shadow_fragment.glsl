#version 140

$$$HEADER$$$

in vec4 position_interp;
out vec4 frag_color;

void main()
{
	float depth = position_interp.z / position_interp.w;
	depth = depth * 0.5 + 0.5;
	
	float moment1 = depth;
	float moment2 = depth * depth;
	
	// Adjusting moments (this is sort of bias per pixel) using partial derivative
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25*(dx*dx+dy*dy);
	
	frag_color = vec4(moment1,moment2,0.0,0.0);
}

