#version 330 core

// For God's sake, it's called "sampler2D"!
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Lamp {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TextureCoordinate;

out vec4 color;

uniform Material material;
uniform Lamp lamp;

void main() {
    // ambient
    vec3 ambient = lamp.ambient * vec3(texture(material.diffuse, TextureCoordinate));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lamp.position - FragmentPosition);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = lamp.diffuse * diff * vec3(texture(material.diffuse, TextureCoordinate));

    // specular
    vec3 viewDirection = normalize(-FragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = lamp.specular * spec * vec3(texture(material.specular, TextureCoordinate));

    // attenuation
    float distance = length(lamp.position - FragmentPosition);
    float attenuation = 1.0f / (lamp.constant + lamp.linear * distance + lamp.quadratic * distance * distance);

    // Combine them together
    vec3 result = attenuation * (ambient + diffuse + specular);
    color = vec4(result, 1.0f);
}
