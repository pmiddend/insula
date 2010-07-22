//#version 140

//out vec4 frag_color;
uniform sampler2D sand,rock,grass;
varying vec2 texcoord;
varying vec2 height_and_gradient_out;
varying float light_intensity;
uniform vec3 sun_position;
varying vec3 eye_space;

varying vec3 normal_out;

const float shinyness = 32.0;
const vec3 ambientColor = vec3(0.05, 0.05, 0.05); //grey
const vec3 diffuseColor = vec3(0.3, 0.6, 0.2); //green
const vec3 specularColor = vec3(0.1, 0.1, 0.1); //more specular looks wired with fog


vec4 blinnPhongReflectance(vec3 N, vec3 L, vec3 V, vec3 lightColor){
  vec3 H = normalize(L+V); // half way vector

  vec3 amb = ambientColor * lightColor;
  vec3 diff = diffuseColor * dot(L, N) * lightColor;
  vec3 spec = specularColor * pow(dot(H, N), shinyness) * lightColor;
  spec = max(spec, vec3(0.0, 0.0, 0.0));

  return vec4(amb + diff + spec, 1.0);
}


void main()
{
	//gl_FragColor = vec4(1.0,1.0,1.0,1.0);

	float 
		grad = height_and_gradient_out.y,
		first = 1.0-height_and_gradient_out.x,
		second = height_and_gradient_out.x;

	vec3 N = normalize(normal_out);
	vec3 L = normalize(sun_position);
	vec3 V = normalize(eye_space * vec3(1.0, 1.0, -1.0));

	vec4 light_color =
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

	gl_FragColor = light_color;
	//gl_FragColor = blinnPhongReflectance(N, L, V, light_color.xyz);
	//gl_FragColor = blinnPhongReflectance(N, L, V, light_color.xyz);
}

