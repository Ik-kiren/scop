#version 330 core

// Ouput data
out vec4 FragColor;

in vec4 vertexPos;
in vec3 vertexColor;
in vec2 TexCoord;

uniform float timeValue;
uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, TexCoord);
}