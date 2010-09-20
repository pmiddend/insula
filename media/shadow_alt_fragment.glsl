#version 140

uniform sampler2D depth_texture;

in vec4 shadow_coord;
in vec2 texcoord_out;
out vec4 frag_color;

float LinearizeDepth(float zoverw)
{
	float n = 1.0; // camera z near
	float f = 10000.0; // camera z far
	return (2.0 * n) / (f + n - zoverw * (f - n));
}


void main()
{
	//vec2 coord = shadow_coord.xy / shadow_coord.w;
	//frag_color = texture2D(depth_texture,coord);
	//frag_color = vec4(shadow_coord.w/30.0,shadow_coord.w/30.0,shadow_coord.w/30.0,1.0);

	//vec2 coord = shadow_coord.xy / shadow_coord.w * 0.5 + 0.5;
	//float value = texture2D(depth_texture,coord).w;
	//frag_color = vec4(value,value,value,1.0);
	//frag_color = vec4(vec3(texture2D(depth_texture,coord)),1.0);
	//frag_color = vec4(vec3(texture2D(depth_texture,texcoord_out)),1.0);
	//frag_color = texture2D(depth_texture,texcoord_out);
	float value = texture2D(depth_texture,texcoord_out).r;
	value = LinearizeDepth(value) * 3;
	//value = (value - border) / (1.0 - border);
	frag_color = vec4(value,value,value,1.0);
	//vec4 shadowCoordinateWdivide = shadow_coord / shadow_coord.w ;
		
	/*
	// Used to lower moiré pattern and self-shadowing
	shadowCoordinateWdivide.z += 0.0005;

	float distanceFromLight = 
		texture2D(
			depth_texture,
			shadowCoordinateWdivide.st).z;

	float shadow = 1.0;
	if (shadow_coord.w > 0.0)
		shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
	*/
	  	
	//frag_color = shadow * vec4(1.0,1.0,1.0,1.0);
	//frag_color = texture2D(depth_texture,shadowCoordinateWdivide.st).z * vec4(1.0,1.0,1.0,1.0);
}
