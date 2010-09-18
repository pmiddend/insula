#version 140

$$$HEADER$$$

in vec4 shadow_coord;
out vec4 frag_color;

void main()
{
	//vec2 coord = shadow_coord.xy / shadow_coord.w;
	//frag_color = texture2D(depth_texture,coord);
	//frag_color = vec4(shadow_coord.w/30.0,shadow_coord.w/30.0,shadow_coord.w/30.0,1.0);

	vec2 coord = shadow_coord.xy / shadow_coord.w * 0.5 + 0.5;
	//float value = texture2D(depth_texture,coord).w;
	//frag_color = vec4(value,value,value,1.0);
	frag_color = vec4(vec3(texture2D(depth_texture,coord)) * 10000000.0,1.0);
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
