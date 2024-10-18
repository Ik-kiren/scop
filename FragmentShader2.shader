#version 330 core

// Ouput data
out vec3 color;
in vec3 vertexPos;

uniform float timeValue;

void main()
{

	// Output color = red 
	color = vertexPos * timeValue;
}