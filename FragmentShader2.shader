#version 330 core

// Ouput data
//out vec3 color;
out vec4 fragText;
in vec3 vertexPos;
in vec3 normal;
in vec3 fragpos;

uniform vec3 cameraPos;

uniform float timeValue;
uniform sampler2D ourTexture;
uniform bool activeTexture;
uniform float timerTextureTransition;

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 color = vec3(1.0, 0.5, 0.31);
	vec3 lightDir = normalize(vec3(10, 10, 10) - fragpos);

	
	// Output color = red 
	//color = vertexPos * timeValue;
	

	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;
	vec2 textpos = vertexPos.xy;
	float diff = max(dot(normalize(normal), lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(cameraPos - fragpos);
    vec3 reflectDir = reflect(-lightDir, normalize(normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * color;
	vec4 tmpTexture = texture(ourTexture, textpos);
	if (activeTexture)
		fragText = vec4(mix(result.x, tmpTexture.x, timerTextureTransition), mix(result.y, tmpTexture.y, timerTextureTransition), mix(result.z, tmpTexture.z, timerTextureTransition), 1.0);
	else
		fragText = vec4(mix(tmpTexture.x, result.x, timerTextureTransition), mix(tmpTexture.y, result.y, timerTextureTransition), mix(tmpTexture.z, result.z, timerTextureTransition), 1.0);
}