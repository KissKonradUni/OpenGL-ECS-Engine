#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 vertexNormal;
out vec2 uvCoords;
out vec3 fragPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);

	vertexNormal = mat3(transpose(inverse(model))) * aNormal;
	uvCoords = aUV;
	fragPos = vec3(model * vec4(aPos, 1.0));
}