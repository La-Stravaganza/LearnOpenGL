#version 330 core
in vec3 vsOutNormal;
in vec3 vsOutFragmentPosition;
in vec3 vsOutLampPosition;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lampColor;

void main() {
    // ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lampColor;

    // diffuse
	vec3 norm = normalize(vsOutNormal);
	vec3 lightDirection = normalize(vsOutLampPosition - vsOutFragmentPosition);
	float diffuseStrength = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diffuseStrength * lampColor;

    // specular
    float specularStrength = 0.5f;
    float shininess = 16;
    vec3 viewDirection = normalize(-vsOutFragmentPosition); // The viewer is at (0, 0, 0)
    vec3 reflectionDirection = reflect(-lightDirection, norm);

    // Phong
    // float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), shininess);

    // Blinn-Phong
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
    float spec = pow(max(dot(norm, halfwayDirection), 0.0), shininess);
    vec3 specular = specularStrength * spec * lampColor;

    // Combine them together
	vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
}
