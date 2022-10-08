#version 330 core
in vec3 vertexNormal;
in vec2 uvCoords;
in vec3 fragPos;

uniform sampler2D texture0;
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
	vec3 objectColor	   = texture(texture0, uvCoords).xyz;
	vec3 lightPos		   = vec3(1.0f, 5.0f, 1.0f);
	vec3 lightColor		   = vec3(1.0f, 1.0f, 1.0f);
	float ambientStrength  = 0.1f;
	float specularStrength = 0.5f;
	float lightConstant    = 1.0f;
	float lightLinear	   = 0.022f;
	float lightQuadratic   = 0.0019f;

	vec3 ambient	  = ambientStrength * lightColor;

	vec3 norm		  = normalize(vertexNormal);
	vec3 lightDir	  = normalize(lightPos - fragPos);

	float diff		  = max(dot(norm, lightDir), 0.0);
	vec3 diffuse	  = diff * lightColor;

	vec3 viewDir	  = normalize(viewPos - fragPos);
	vec3 reflectDir   = reflect(-lightDir, norm);
	
	float spec		  = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular	  = specularStrength * spec * lightColor;

	float distance    = length(lightPos - fragPos);
	float attenuation = 1.0 / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4(result, 1.0f);
}