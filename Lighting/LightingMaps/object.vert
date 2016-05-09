#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinate;

out vec3 vsOutNormal;
out vec3 vsOutFragmentPosition;
out vec3 vsOutLampPosition;
out vec2 vsOutTextureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lampPosition;

// Compute Phong illumination in the View Space
void main() {
    gl_Position = projection * view * model * vec4(position, 1.0f);

    // output the fragment position in the view space
    vsOutFragmentPosition = vec3(view * model * vec4(position, 1.0f));

    // output the normal position in the view space
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vsOutNormal = normalMatrix * normal;

    // output the lamp position in the view space
    vsOutLampPosition = vec3(view * vec4(lampPosition, 1.0));

    // output the texture coordinate
    vsOutTextureCoordinate = textureCoordinate;
}
