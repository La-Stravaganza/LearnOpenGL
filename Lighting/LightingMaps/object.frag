#version 330 core

// For God's sake, it's called "sampler2D"!
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Lamp {
    // vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 vsOutNormal;
in vec3 vsOutFragmentPosition;
in vec3 vsOutLampPosition;
in vec2 vsOutTextureCoordinate;

out vec4 color;

uniform Material material;
uniform Lamp lamp;

void main() {
    // ambient
    vec3 ambient = lamp.ambient * vec3(texture(material.diffuse, vsOutTextureCoordinate));

    // diffuse
    vec3 norm = normalize(vsOutNormal);
    vec3 lightDirection = normalize(vsOutLampPosition - vsOutFragmentPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = lamp.diffuse * diff * vec3(texture(material.diffuse, vsOutTextureCoordinate));

    // specular
    vec3 viewDirection = normalize(-vsOutFragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = lamp.specular * spec * vec3(texture(material.specular, vsOutTextureCoordinate));

    // Combine them together
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}
