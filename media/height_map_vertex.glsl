#version 140

$$$HEADER$$$
out vec2 texcoord_interp;
out vec2 height_gradient_interp;
out vec4 shadow_coord_interp;
//out float light_intensity_interp;

void main()
{
	texcoord_interp = 
		texture_scaling * position.xz / grid_size;
	
	gl_PointSize = 
	//light_intensity = 
		ambient_light + 
		max(
			0.0, 
			dot(normal, sun_direction)) + float(height_and_gradient);

	height_gradient_interp = height_and_gradient;

	gl_Position = mvp * vec4(position,1.0);
	shadow_coord_interp = shadow_mvp * vec4(position,1.0);

	if (do_clip == 1)
		gl_ClipDistance[0] = position.y - water_level;
}
