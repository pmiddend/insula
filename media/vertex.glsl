//#version 140

uniform mat4 mvp;

void main()
{
	gl_Position = 
		transpose(mvp) *
		gl_Vertex;
}
