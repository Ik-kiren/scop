#version 330 core

// Ouput data
//out vec3 color;
out vec4 fragText;
in vec3 vertexPos;

uniform float timeValue;
uniform sampler2D ourTexture;

void main()
{

	// Output color = red 
	//color = vertexPos * timeValue;
	vec2 textpos = vertexPos.xy;
	fragText = texture(ourTexture, textpos);
}