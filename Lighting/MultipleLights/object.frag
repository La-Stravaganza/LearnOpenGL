#version 330 core

// For God's sake, it's called "sampler2D"!
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutoff; // in cosine
    float outerCutoff; // in cosine
};

in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TextureCoordinate;

out vec4 color;

const int NumberOfPointLights = 4;

uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NumberOfPointLights];
uniform SpotLight spotLight;

vec3 calculate_directional_light(DirectionalLight directionalLight);
vec3 calculate_point_light(PointLight pointLight);
vec3 calculate_spot_light(SpotLight spotLight);

void main() {
    vec3 result = calculate_directional_light(directionalLight);
    for (int i = 0; i < NumberOfPointLights; ++i) {
        result += calculate_point_light(pointLights[i]);
    }
    result += calculate_spot_light(spotLight);
    color = vec4(result, 1.0f);
}

vec3 calculate_directional_light(DirectionalLight directionalLight) {
    // ambient
    vec3 ambient = directionalLight.ambient * vec3(texture(material.diffuse, TextureCoordinate));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-directionalLight.direction); // pointing from the fragment to the lamp
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = directionalLight.diffuse * diff * vec3(texture(material.diffuse, TextureCoordinate));

    // specular
    vec3 viewDirection = normalize(-FragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = directionalLight.specular * spec * vec3(texture(material.specular, TextureCoordinate));

    // Combine them together
    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 calculate_point_light(PointLight pointLight) {
    // ambient
    vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, TextureCoordinate));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(pointLight.position - FragmentPosition); // pointing from the fragment to the lamp
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = pointLight.diffuse * diff * vec3(texture(material.diffuse, TextureCoordinate));

    // specular
    vec3 viewDirection = normalize(-FragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = pointLight.specular * spec * vec3(texture(material.specular, TextureCoordinate));

    // attenuation
    float distance = length(pointLight.position - FragmentPosition);
    float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);

    // Combine them together
    vec3 result = attenuation * (ambient + diffuse + specular);
    return result;
}

vec3 calculate_spot_light(SpotLight spotLight){
    // ambient
    vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, TextureCoordinate));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(spotLight.position - FragmentPosition); // pointing from the fragment to the lamp
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.diffuse, TextureCoordinate));

    // specular
    vec3 viewDirection = normalize(-FragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, TextureCoordinate));

    // intensity
    float thetaCosine = dot(lightDirection, normalize(-spotLight.direction));
    float epsilon = spotLight.cutoff - spotLight.outerCutoff;
    float intensity = clamp((thetaCosine - spotLight.outerCutoff) / epsilon, 0.0f, 1.0f);

    // Combine them together
    vec3 result = ambient + intensity * diffuse + intensity * specular;
    return result;
}
