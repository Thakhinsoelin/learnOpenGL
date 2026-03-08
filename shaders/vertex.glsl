#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 vNormal;
out vec3 vFragPos;
out vec2 vTexCoords;
//out vec2 ScreenUV;
//uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
//	gl_Position = vec4(aPos.x + mouseOffset.x, aPos.y + mouseOffset.y, aPos.z, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	vNormal = aNormal;
	vFragPos = vec3(model * vec4(aPos, 1.0f));
	vTexCoords = aTexCoords;
	//TexCoord = aTexCoord;
	//ScreenUV = aPos.xy;
}