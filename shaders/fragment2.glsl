#version 330 core
out vec4 FragColor;

in vec3 vNormal;
in vec3 vFragPos;

in vec2 vTexCoords;

struct Material {
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	float ambientStrength = 0.1f;
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vTexCoords));

	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(lightPos - vFragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vTexCoords));


	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - vFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0f);
}