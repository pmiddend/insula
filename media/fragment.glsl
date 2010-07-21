//#version 140

//out vec4 frag_color;
uniform sampler2D sand,rock,grass;
varying vec2 texcoord;
varying vec2 height_and_gradient_out;
varying float light_intensity;

void main()
{
	//gl_FragColor = vec4(1.0,1.0,1.0,1.0);

	float 
		grad = height_and_gradient_out.y,
		first = 1.0-height_and_gradient_out.x,
		second = height_and_gradient_out.x;

	gl_FragColor = 
		light_intensity * 
		(
			(1.0-grad) * first * 
			texture2D(
				sand,
				texcoord) +
			(1.0-grad) * second * 
			texture2D(
				grass,
				texcoord) +
			grad *
			texture2D(
				rock,
				texcoord));
}
