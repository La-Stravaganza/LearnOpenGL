#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragmentPosition;
out vec2 TextureCoordinate;

// Compute Phong illumination in the View Space
void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);

    // output the normal position in the view space
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    Normal = normalMatrix * normal;

    // output the fragment position in the view space
    FragmentPosition = vec3(view * model * vec4(position, 1.0f));

    // output the texture coordinate
    TextureCoordinate = textureCoordinate;
}
