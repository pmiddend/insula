//#version 140

//out vec4 frag_color;
uniform sampler2D main_texture;
varying vec3 position;

void main()
{
	//gl_FragColor = vec4(1.0,1.0,1.0,1.0);

	gl_FragColor = 
		texture2D(
			main_texture,
			vec2(
				position.x/2560.0,
				position.z/2560.0));
}
