#version 330 core
in vec3 vsOutColor;
in vec2 vsOutTextureCoordinate;

out vec4 fsOutColor;
uniform sampler2D uniformTexture;

void main() {
	fsOutColor = texture(uniformTexture, vsOutTextureCoordinate) * vec4(vsOutColor, 1.0f);
}
