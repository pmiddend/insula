#version 140

uniform mat4 mvp;
in vec3 pos;

void main()
{
	// Hier mal die mvp-Matrix explizit hingeschrieben
	//gl_Position = mat4(1.68616,0,-0.659824,0,-0.410932,2.13467,-1.05012,0,-0.393818,-0.570891,-1.00639,-2.22222,-0.322215,-0.467093,-0.823409,0) * vec4(pos,1.0);
	gl_Position = mvp * vec4(pos,1.0);
}
