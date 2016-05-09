#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Lamp {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vsOutNormal;
in vec3 vsOutFragmentPosition;
in vec3 vsOutLampPosition;

out vec4 color;

uniform Material material;
uniform Lamp lamp;

void main() {
    // ambient
    vec3 ambient = lamp.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(vsOutNormal);
    vec3 lightDirection = normalize(vsOutLampPosition - vsOutFragmentPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = lamp.diffuse * (diff * material.diffuse);

    // specular
    vec3 viewDirection = normalize(-vsOutFragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = lamp.specular * (spec * material.specular);

    // Combine them together
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}
