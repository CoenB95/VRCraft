#version 330

//Uniforms

//Inputs (from geometry/vertex-shader)

//Outputs (to draw buffers)
out float fragmentDepth;

//Shader
void main()
{
	fragmentDepth = gl_FragCoord.z;
}