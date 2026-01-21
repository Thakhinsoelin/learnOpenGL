#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec2 ScreenUV;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float time;


void main()
{
	//vec2 screenUV = gl_FragCoord.xy / vec2(800.0, 600.0);
	//vec4 texColor = texture(texture2, screenUV);
	//float dist = distance(TexCoord, vec2(0.5,0.5));
	//float brightness = smoothstep(0.5,0.2,dist);
	//FragColor = texColor * brightness;
	//FragColor = texture(texture2, screenUV);
	//float flicker = 1.0 - (sin(time * 20.0) * 0.1);
	FragColor = mix(texture(texture1, TexCoord),
					texture(texture2, vec2(1.0-TexCoord.x, 1.0-TexCoord.y)),0.2);
}