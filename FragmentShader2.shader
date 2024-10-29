#version 330 core

// Ouput data
//out vec3 color;
out vec4 fragText;
in vec3 vertexPos;

uniform float timeValue;
uniform sampler2D ourTexture;
uniform bool activeTexture;

void main()
{

	// Output color = red 
	//color = vertexPos * timeValue;
	vec2 textpos = vertexPos.xy;
	if (activeTexture)
		fragText = texture(ourTexture, textpos);
}