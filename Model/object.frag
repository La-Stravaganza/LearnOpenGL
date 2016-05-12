#version 330 core

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TextureCoordinate;

out vec4 color;

uniform Material material;
uniform PointLight pointLight;

vec3 calculate_point_light(PointLight pointLight);

void main() {
     vec3 result = calculate_point_light(pointLight);
     color = vec4(result, 1.0f);
}

vec3 calculate_point_light(PointLight pointLight) {
    // ambient
    vec3 ambient = pointLight.ambient * vec3(texture(material.texture_diffuse1, TextureCoordinate));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(pointLight.position - FragmentPosition); // pointing from the fragment to the lamp
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = pointLight.diffuse * diff * vec3(texture(material.texture_diffuse1, TextureCoordinate));

    // specular
    vec3 viewDirection = normalize(-FragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = pointLight.specular * spec * vec3(texture(material.texture_specular1, TextureCoordinate));

    // attenuation
    // float distance = length(pointLight.position - FragmentPosition);
    // float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);

    // Combine them together
    // vec3 result = attenuation * (ambient + diffuse + specular);
    vec3 result = ambient + diffuse + specular;
    return result;
}

