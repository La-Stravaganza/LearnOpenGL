#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 textureCoordinate;

out vec3 vsOutColor;
out vec2 vsOutTextureCoordinate;

void main() {
    gl_Position = vec4(position, 1.0f);
    vsOutColor = color;

    // upside down the coordinates of images
    vsOutTextureCoordinate = vec2(textureCoordinate.x, 1 - textureCoordinate.y);
}
