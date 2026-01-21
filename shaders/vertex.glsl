#version 330 core 
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out vec2 ScreenUV;
uniform vec2 mouseOffset;

void main()
{
//	gl_Position = vec4(aPos.x + mouseOffset.x, aPos.y + mouseOffset.y, aPos.z, 1.0);
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
	ScreenUV = aPos.xy;
}